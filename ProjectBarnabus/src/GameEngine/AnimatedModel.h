#pragma once
#include "Entity.h"
#include "Joint.h"
#include "MeshData.h"

//class Animator;

class AnimatedModel : public Component
{
public:

	AnimatedModel(/*Texture texture,*/ Joint rootJoint, int jointCount);

	void LoadModel(std::string fileName);


	std::vector<glm::mat4> GetJointTransforms();
	void AddJointsToArray(Joint& headJoint, std::vector<glm::mat4>& jointMatrices);

	void Update(double delta) override;
	void Render() override;



	//Skin 
	MeshData model;
	//Texture texture;

	// Skeleton
	Joint rootJoint;
	int jointCount;

	//Animator animator;
};