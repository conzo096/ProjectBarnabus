#pragma once

#include <string>
#include <glm/glm.hpp>

class Bone
{
public:
	std::string name;
	glm::mat4 offsetMatrix;
	glm::mat4 finalTransformation;

};