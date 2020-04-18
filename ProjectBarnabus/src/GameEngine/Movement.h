#pragma once
#include "Entity.h"
#include "Terrain.h"
#include "Camera.h"

class Movement : public Component
{
public:
	Movement();
	Movement(Terrain* const entityTerrain, Camera* const entityCamera);
	~Movement();

	void SetTerrain(Terrain* const entityTerrain);

	void Update(float deltaTime) override;

private:
	Terrain* terrain;
	Camera* camera;
};

