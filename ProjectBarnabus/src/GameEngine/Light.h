#pragma once
#include "glm/glm.hpp"
#include <string>

class Light
{
public:
	Light();
	~Light();

	void SetName(std::string lightName);
	std::string GetName();
protected:
	std::string name;
	glm::vec4 colour;
};

