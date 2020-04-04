#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

protected:
	glm::vec3 direction;
};

