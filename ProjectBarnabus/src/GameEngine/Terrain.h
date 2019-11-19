#pragma once
#include "Model.h"
#include <Math.h>
#include <limits>

enum TerrainType
{
	Image,
	Model
};

class Terrain : public Model
{
public:

	Terrain();
	Terrain(const std::string filePath, TerrainType terrain);

	glm::vec3 GetWorldPositionFromGrid(glm::vec3 worldPosition);
	void LoadTerrainFromHeightMap(const std::string heightMapPath);

private:
	float** heightPositionsGrid;
	int width = 0, height = 0;
};

