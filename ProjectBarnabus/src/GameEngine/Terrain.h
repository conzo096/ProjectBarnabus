#pragma once
#include "Model.h"
#include "PhysicsContainer.h"
#include <Math.h>
#include <limits>

class Terrain : public Model
{
public:
	enum TerrainType
	{
		Image,
		Model
	};

	struct Grid 
	{
		float height;
		bool occupied = false;
	};
public:
	Terrain();
	glm::vec3 GetWorldPositionFromGrid(glm::vec3 worldPosition);

	void LoadTerrainFromHeightMap(const std::string heightMapPath);

	bool IsTerrainValid(Physics::PhysicsContainer& const physicsContainer);

	void UpdateTerrain(Physics::PhysicsContainer& const physicsContainer, bool occupied);

private:
	glm::ivec2 GetGridSquare(glm::vec3 position);
private:
	Grid** heightPositionsGrid;

	int width = 0, m_length = 0;
};

