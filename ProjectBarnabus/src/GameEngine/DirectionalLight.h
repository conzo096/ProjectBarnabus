#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec4 lightColour, glm::vec3 lightposition);
	~DirectionalLight();

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 dir);
protected:
	glm::vec3 position;
};

