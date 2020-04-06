#include "Light.h"



Light::Light()
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
