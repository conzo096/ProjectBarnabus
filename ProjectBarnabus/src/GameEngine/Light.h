#pragma once
#include "glm/glm.hpp"
#include <string>

class Light
{
public:
	Light();
	Light(glm::vec4 lightColour);
	~Light();

	void SetName(std::string lightName);
	std::string GetName();

	void SetColour(glm::vec4 lightColour);
	glm::vec4 GetColour();
protected:
	std::string name;
	glm::vec4 colour;
};

