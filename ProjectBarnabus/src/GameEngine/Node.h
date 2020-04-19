#pragma once
#include "MeshData.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

struct Node
{
	std::string name;
	std::vector<Node*> children;
	std::vector<MeshData> data;
	glm::mat4 transformation = glm::mat4(1.0);
	Node* parent;

};