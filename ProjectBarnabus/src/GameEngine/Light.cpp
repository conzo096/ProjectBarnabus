#include "Light.h"



Light::Light()
{
}

Light::Light(glm::vec4 lightColour) : colour(lightColour)
{
}


Light::~Light()
{
}

void Light::SetName(std::string lightName)
{
	name = lightName;
}

std::string Light::GetName()
{
	return name;
}

void Light::SetColour(glm::vec4 lightColour)
{
	colour = lightColour;
}

glm::vec4 Light::GetColour()
{
	return colour;
}
