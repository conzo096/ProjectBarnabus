#include "AnimatedModel.h"
//#include "Animator.h"

AnimatedModel::AnimatedModel(/*Texture texture,*/ Joint rootJoint, int jointCount) : Component("AnimatedModel")
{
	this->model = model;
	//this.texture = texture;
	this->rootJoint = rootJoint;
	this->jointCount = jointCount;
	//this->animator = Animator(this);
	rootJoint.CalculateInverseBindTransform(glm::mat4());
}

void AnimatedModel::LoadModel(std::string fileName)
{


}

std::vector<glm::mat4> AnimatedModel::GetJointTransforms()
{
	std::vector<glm::mat4> jointMatrices;

	return jointMatrices;
}

void AnimatedModel::AddJointsToArray(Joint & headJoint, std::vector<glm::mat4>& jointMatrices)
{
	jointMatrices[headJoint.index] = headJoint.GetTransform();
	for (Joint childJoint : headJoint.children)
	{
		AddJointsToArray(childJoint, jointMatrices);
	}
}

void AnimatedModel::Update(double delta)
{
}

void AnimatedModel::Render()
{
}
