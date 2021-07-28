#include "Terrain.h"
#include "OpenGLHeightMapTexture.h"
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

glm::vec3 Terrain::GetWorldPositionFromGrid(glm::vec3 worldPosition)
{
	glm::vec3 destination = worldPosition;


	auto gridPos = GetGridSquare(worldPosition);
	int gridX = gridPos.x;
	int gridZ = gridPos.y;
	if (gridX >= m_length - 1 || gridZ >= m_length - 1 ||
		gridX < 0 || gridZ < 0)
	{
		return worldPosition;
	}

	float terrainX = worldPosition.x - GetParent()->GetPosition().x;
	float terrainZ = worldPosition.z - GetParent()->GetPosition().z;
	float gridSquare = m_length / (m_length - 1);
	float xCoord = fmodf(terrainX, gridSquare);
	float zCoord = fmodf(terrainZ, gridSquare);

	float yAnswer = 0;
	if (xCoord <= (1 - zCoord))
	{
		yAnswer = BarryCentric(glm::vec3(0, heightPositionsGrid[gridX][gridZ].height, 0), glm::vec3(1,
			heightPositionsGrid[gridX + 1][gridZ].height, 0), glm::vec3(0,
				heightPositionsGrid[gridX][gridZ + 1].height, 1), glm::vec2(xCoord, zCoord));
	}
	else
	{
		yAnswer = BarryCentric(glm::vec3(1, heightPositionsGrid[gridX + 1][gridZ].height, 0), glm::vec3(1,
			heightPositionsGrid[gridX + 1][gridZ + 1].height, 1), glm::vec3(0,
				heightPositionsGrid[gridX][gridZ + 1].height, 1), glm::vec2(xCoord, zCoord));
	}

	destination.y = (yAnswer * GetParent()->GetScale().y) + GetParent()->GetPosition().y;
	return destination;
}

void Terrain::LoadTerrainFromHeightMap(const std::string heightMapPath)
{
	auto getHeight = [](auto x, auto z, HeightMapTexture& heightMap)
	{
		const unsigned int maxColour = 255 * 255 * 255;
		int r = heightMap.GetData()[x * 4 + 0 + z * 4 * heightMap.GetWidth()];
		int g = heightMap.GetData()[x * 4 + 1 + z * 4 * heightMap.GetWidth()];
		int b = heightMap.GetData()[x * 4 + 2 + z * 4 * heightMap.GetWidth()];
		int a = heightMap.GetData()[x * 4 + 3 + z * 4 * heightMap.GetWidth()];
		unsigned int argb = r*g*b*a;

		return ((float)argb / (float)maxColour);
	};

	HeightMapTexture heightMap(heightMapPath);
	width = heightMap.GetWidth();
	m_length = heightMap.GetHeight();
	std::vector<Vertex> vertices;

	heightPositionsGrid = new Grid*[heightMap.GetWidth()];

	for (int i = 0; i < heightMap.GetWidth(); i++)
	{
		heightPositionsGrid[i] = new Grid[heightMap.GetHeight()];
	}

	MeshData mesh;
	for (int z = 0; z < m_length; z++)
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
			if (x < width - 1 && z < m_length - 1) {
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

			heightPositionsGrid[x][z].height = vertex.position.y;
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

	mesh.SetType(MeshData::TRIANGLE);

	rootNode = new Node;
	rootNode->name = "Terrain";
	rootNode->data.push_back(mesh);

	BarnabusGameEngine::Get().AddMessageLog(StringLog("Terrain created from: " + heightMapPath, StringLog::Priority::Low));
}

bool Terrain::IsTerrainValid(Physics::PhysicsContainer& const physicsContainer)
{
	auto boundingBoxes = physicsContainer.GetBoundingVolume()->GetBoundingBoxes();

	for (auto bb : boundingBoxes)
	{
		auto min = bb.GetMinCoordinates();
		auto range = bb.GetMaxCoordinates() - min;

		for (int x = min.x; x < min.x + range.x; x++)
		{
			for (int z = min.z; z < min.z + range.z; z++)
			{
				// Compare against grid. 

				auto position = GetWorldPositionFromGrid({ x,min.y,z });
				auto gridPos = GetGridSquare({ x,min.y,z });
				// Check that the position is within tolorance and
				// Grid is not already occupied.
				if (std::abs(position.y - min.y) > 5.0 || heightPositionsGrid[gridPos.x][gridPos.y].occupied)
				{
					return false;
				}
			}
		}
	}
	return true;
}

void Terrain::UpdateTerrain(Physics::PhysicsContainer& const physicsContainer, bool occupied)
{
	auto boundingBoxes = physicsContainer.GetBoundingVolume()->GetBoundingBoxes();

	for (auto bb : boundingBoxes)
	{
		auto min = bb.GetMinCoordinates();
		auto range = bb.GetMaxCoordinates() - min;

		for (int x = min.x; x < min.x + range.x; x++)
		{
			for (int z = min.z; z < min.z + range.z; z++)
			{
				auto gridPos = GetGridSquare({ x,min.y,z });
				heightPositionsGrid[gridPos.x][gridPos.y].occupied = occupied;
			}
		}
	}
}

glm::ivec2 Terrain::GetGridSquare(glm::vec3 position)
{
	// Convert world space to local space. This needs to take into account the offset. 
	float terrainX = position.x - GetParent()->GetPosition().x;
	float terrainZ = position.z - GetParent()->GetPosition().z;

	float gridSquare = m_length / (m_length - 1);
	int gridX = (int)floor(terrainX / gridSquare);
	int gridZ = (int)floor(terrainZ / gridSquare);

	return { gridX, gridZ };
}
