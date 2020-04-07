#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec4 lightColour, glm::vec3 lightPosition) : Light(lightColour), position(lightPosition)
{
}

DirectionalLight::~DirectionalLight()
{
}

glm::vec3 DirectionalLight::GetPosition()
{
	return position;
};

void DirectionalLight::SetPosition(glm::vec3 dir)
{
	position = dir;
}