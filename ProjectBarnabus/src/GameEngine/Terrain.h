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
	double GetWorldHeightPositionFromGrid(glm::vec3 worldPosition);

private:
	float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
private:
	float** heightPositionsGrid;
	int width = 0, height = 0;
	void LoadTerrainFromHeightMap(const std::string heightMapPath);
};

