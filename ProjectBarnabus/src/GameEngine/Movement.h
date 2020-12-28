#pragma once
#include "Entity.h"
#include "Terrain.h"
#include "Camera.h"

class Movement : public Component
{
public:
	Movement();
	Movement(Terrain* const entityTerrain);
	~Movement();

	void SetTerrain(Terrain* const entityTerrain);

	void Update(float deltaTime) override;

	void SetMovement(glm::vec3 move);
private:
	Terrain* terrain;
	glm::vec3 movement;
};

