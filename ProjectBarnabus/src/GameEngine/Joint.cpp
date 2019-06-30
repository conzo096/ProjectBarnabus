#include "Joint.h"

Joint::Joint()
{
}

Joint::Joint(int index, std::string name, glm::mat4 bindLocalTransform)
{
	this->index = index;
	this->name = name;
	this->localBindTransform = bindLocalTransform;
}

void Joint::AddChild(Joint child)
{
	children.push_back(child);
}

void Joint::CalculateInverseBindTransform(glm::mat4 parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform * localBindTransform;
	inverseBindTransform = glm::inverse(bindTransform);

	for (Joint child : children)
	{
		child.CalculateInverseBindTransform(bindTransform);
	}
}
