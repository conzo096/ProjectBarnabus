#include "Terrain.h"
#include "HeightMapTexture.h"
#include "BarnabusGameEngine.h"
#include <vector>

namespace
{
float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
}

Terrain::Terrain()
{
	token = "Terrain";
}

Terrain::Terrain(const std::string filePath, TerrainType terrain) : Terrain()
{
	LoadTerrainFromHeightMap(filePath);
}

glm::vec3 Terrain::GetWorldPositionFromGrid(glm::vec3 worldPosition)
{
	glm::vec3 convertedPosition = glm::vec4(worldPosition, 1) * glm::inverse(GetTransform());
	glm::vec3 destination = worldPosition;
	
	// Convert world space to local space.
	float terrainX = convertedPosition.x - GetPosition().x;
	float terrainZ = convertedPosition.z - GetPosition().z;

	float gridSquare = height / (height - 1);
	int gridX = (int)floor(terrainX / gridSquare);
	int gridZ = (int)floor(terrainZ / gridSquare);
	if (gridX >= height - 1 || gridZ >= height - 1 ||
		gridX < 0 || gridZ < 0)
	{
		return worldPosition;
	}

	float xCoord = fmodf(terrainX, gridSquare);
	float zCoord = fmodf(terrainZ, gridSquare);
	float yAnswer = 0;
	if (xCoord <= (1 - zCoord))
	{
		yAnswer = BarryCentric(glm::vec3(0, heightPositionsGrid[gridX][gridZ], 0), glm::vec3(1,
			heightPositionsGrid[gridX + 1][gridZ], 0), glm::vec3(0,
				heightPositionsGrid[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}
	else
	{
		yAnswer = BarryCentric(glm::vec3(1, heightPositionsGrid[gridX + 1][gridZ], 0), glm::vec3(1,
			heightPositionsGrid[gridX + 1][gridZ + 1], 1), glm::vec3(0,
				heightPositionsGrid[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}

	destination.y = yAnswer * GetParent()->GetScale().y;
	return destination;
}

void Terrain::LoadTerrainFromHeightMap(const std::string heightMapPath)
{
	auto getHeight = [](auto x, auto z, HeightMapTexture& heightMap)
	{
		const unsigned int maxColour = 255 * 255 * 255;
		int r = heightMap.GetData()[x * 4 + 0 + z * 4 *  heightMap.GetWidth()];
		int g = heightMap.GetData()[x * 4 + 1 + z * 4 * heightMap.GetWidth()];
		int b = heightMap.GetData()[x * 4 + 2 + z * 4 * heightMap.GetWidth()];
		int a = heightMap.GetData()[x * 4 + 3 + z * 4 * heightMap.GetWidth()];
		unsigned int argb = r*g*b*a;
		
		return ((float)argb / (float)maxColour);
	};

	HeightMapTexture heightMap(heightMapPath);
	width = heightMap.GetWidth();
	height = heightMap.GetHeight();
	std::vector<Vertex> vertices;

	heightPositionsGrid = new float*[heightMap.GetWidth()];

	for (int i = 0; i < heightMap.GetWidth(); i++)
	{
		heightPositionsGrid[i] = new float[heightMap.GetHeight()];
	}

	MeshData mesh;
	for (int z = 0; z < height; z++)
	{
		for (int x = 0; x < width; x++)
		{
			float heightPosition = getHeight(x,z,heightMap);

			Vertex vertex;
			vertex.position = glm::vec3(float(x), heightPosition, float(z));
			vertex.texCoords = glm::vec2(x / heightMap.GetWidth(), z / heightMap.GetHeight());
			vertex.color = glm::vec4(0.34, 0.23, 0.04, 1);

			vertices.push_back(vertex);

			// Create indices
			if (x < width - 1 && z < height - 1) {
				int leftTop = z * width + x;
				int leftBottom = (z + 1) * width + x;
				int rightBottom = (z + 1) * width + x + 1;
				int rightTop = z * width + x + 1;

				mesh.InsertIndex(rightTop);
				mesh.InsertIndex(leftBottom);
				mesh.InsertIndex(leftTop);

				mesh.InsertIndex(rightBottom);
				mesh.InsertIndex(leftBottom);
				mesh.InsertIndex(rightTop);
			}

			heightPositionsGrid[x][z] = (glm::vec4(vertex.position, 1) * GetTransform()).y;
		}
	}

	// calculate normal for terrain
	for (int i = 0; i < vertices.size()-2; i+=3)
	{
		glm::vec3 normal = glm::normalize((vertices[i + 1].position - vertices[i].position) * (vertices[i + 2].position - vertices[i].position));
		for (int j = 0; j < 3; j++)
		{
			vertices[i + j].normal = normal;
		}
	}

	mesh.InsertVertices(vertices);

	int vertexCounter = 0;
	for (int z = 0; z < heightMap.GetHeight() - 1; z++)
	{
		for (int x = 0; x < (heightMap.GetWidth()-1); x++)
		{
			mesh.InsertIndex(vertexCounter + x + heightMap.GetWidth());
			mesh.InsertIndex(vertexCounter + x + (heightMap.GetWidth() + 1));
			mesh.InsertIndex(vertexCounter + x + 1);

			mesh.InsertIndex(vertexCounter + x + heightMap.GetWidth());
			mesh.InsertIndex(vertexCounter + x + 1);
			mesh.InsertIndex(vertexCounter + x);
		}
		vertexCounter += heightMap.GetHeight();
	}

	mesh.SetType(GL_TRIANGLES);

	rootMeshNode = new MeshNode;
	rootMeshNode->name = "Terrain";
	rootMeshNode->data.push_back(mesh);

	InitModel();
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Terrain created from: " + heightMapPath, StringLog::Priority::Low));
}