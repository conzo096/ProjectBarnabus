#pragma once
#include "Entity.h"
#include "Terrain.h"

class Movement : public Component
{
public:
	Movement();
	Movement(Terrain* const entityTerrain);
	~Movement();

	void SetTerrain(Terrain* const entityTerrain);

	void Update(float deltaTime) override;

private:
	Terrain* terrain;
};

