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
	MeshData mesh;

	HeightMapTexture heightMap(heightMapPath);
	width = heightMap.GetWidth();
	height = heightMap.GetHeight();
	std::vector<Vertex> vertices(heightMap.GetWidth()*heightMap.GetHeight());
	int counter = 0;

	heightPositionsGrid = new float*[heightMap.GetWidth()];

	for (int x = 0; x < heightMap.GetWidth(); x++)
	{		
		heightPositionsGrid[x] = new float[heightMap.GetWidth()];
		for (int z = 0; z < heightMap.GetHeight(); z++)
		{
			int index = z * heightMap.GetHeight() + x;
			vertices.at(index).position = glm::vec3(float(x), (float)heightMap.GetData()[index], float(z));
			vertices.at(index).texCoords = glm::vec2(x/heightMap.GetWidth(),z/heightMap.GetHeight());

			float heightPosition = vertices.at(index).position.y / 255;
			vertices.at(index).color = glm::vec4(heightPosition, heightPosition, heightPosition,1);
			heightPositionsGrid[x][z] = (glm::vec4(vertices.at(index).position,1) * GetTransform()).y;
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
	data.push_back(mesh);

	InitModel();
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Terrain created from: " + heightMapPath, StringLog::Priority::Low));
}