#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class Node
{
public:

	std::string name;
	std::vector<Node*> children;
	glm::mat4 transformation;
	Node* parent;

	Node* FindNode(std::string name);
};