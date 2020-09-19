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
	Terrain(const std::string filePath, TerrainType terrain);
	glm::vec3 GetWorldPositionFromGrid(glm::vec3 worldPosition);

private:
	void LoadTerrainFromHeightMap(const std::string heightMapPath);

private:
	float** heightPositionsGrid;
	int width = 0, height = 0;
};

