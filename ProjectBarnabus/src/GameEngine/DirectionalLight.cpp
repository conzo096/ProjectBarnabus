#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec4 lightColour, glm::vec3 lightDirection) : Light(lightColour), direction(lightDirection)
{
}

DirectionalLight::~DirectionalLight()
{
}

glm::vec3 DirectionalLight::GetDirection()
{
	return direction;
};

void DirectionalLight::SetDirection(glm::vec3 dir)
{
	direction = dir;
}