#pragma once
#include "Model.h"
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
public:
	Terrain();
	glm::vec3 GetWorldPositionFromGrid(glm::vec3 worldPosition);

	void LoadTerrainFromHeightMap(const std::string heightMapPath);

private:
	float** heightPositionsGrid;
	std::vector < std::vector<float>> m_grid;

	int width = 0, m_length = 0;
};

