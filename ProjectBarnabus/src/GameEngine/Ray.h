#pragma once
#include <glm/glm.hpp>
#include "Entity.h"
#include "BoundingBox.h"

class Ray
{
public:
	Ray();
	Ray(glm::vec3 pos, glm::vec3 dir, float len);
public:
	void SetPosition(glm::vec3 pos);
	void SetDirection(glm::vec3 dir);
	void SetLength(float len);
public:
	bool IsCollision(Entity& entity, glm::vec3& poi);

private:
	bool TestBoundingBoxCollision(BoundingVolumes::BoundingBox bb);
private:
	glm::vec3 position;
	glm::vec3 direction;
	float length;
};