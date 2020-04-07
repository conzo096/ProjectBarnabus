#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec4 lightColour, glm::vec3 lightDirection);
	~DirectionalLight();

	glm::vec3 GetDirection();
	void SetDirection(glm::vec3 dir);
protected:
	glm::vec3 direction;
};

