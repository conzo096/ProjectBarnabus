#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

struct VertexWeight
{
	unsigned int weight;
	unsigned int vertexId;
};

struct Bone
{
	std::string name;
	glm::mat4 offSet;
	std::vector<VertexWeight> weights;

	glm::mat4 finalTransformation;
};