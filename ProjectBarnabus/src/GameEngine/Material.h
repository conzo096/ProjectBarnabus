#pragma once
#include <glm/glm.hpp>

struct Material
{
	glm::vec4 emissive = glm::vec4(1);
	glm::vec4 diffuse = glm::vec4(0, 0, 0, 1);;
	glm::vec4 specular = glm::vec4(0, 0, 0, 1);;
	float shininess = 0;
};