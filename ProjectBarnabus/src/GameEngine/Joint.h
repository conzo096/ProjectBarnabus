#pragma once

#include "Transform.h"

#include <list>

class Joint : public Transform
{
public:
	Joint();
	Joint(int index, std::string name, glm::mat4 bindLocalTransform);
	void AddChild(Joint child);
	void CalculateInverseBindTransform(glm::mat4 parentBindTransform);

	std::list<Joint> children;
	int index;
	std::string name;

	glm::mat4 localBindTransform; // Only used to calculate below?
	glm::mat4 inverseBindTransform;
};