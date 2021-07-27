#pragma once
#include "Model.h"
#include "BoundingBox.h"
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

	bool IsTerrainValid(BoundingVolumes::BoundingBox& const boundingBox);
private:
	Grid** heightPositionsGrid;

	int width = 0, m_length = 0;
};

