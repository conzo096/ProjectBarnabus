#pragma once
#include "Model.h"
#include "Node.h"
#include "NodeAnim.h"
#include "Animation.h"
#include <map>

class aiNode;

class AnimatedModel : public Model
{
public:

	AnimatedModel(const std::string& fileName);

	void Update(double deltaTime) override;

	void ReadNodeHeirarchy(float AnimationTime, const Node* pNode, const glm::mat4& ParentTransform);

	const NodeAnim* FindNodeAnim(const Animation* pAnimation, const std::string NodeName);


	std::vector<Bone> bones;
	std::map<std::string, int> boneMapping;

	glm::mat4 m_GlobalInverseTransform;
	std::vector<Animation*> animations;

	// Not implemented.
	Node* rootNode;
	float totalTime;
private:
	void LoadNodeTree(Node*& myRootNode, aiNode* rootNode,Node* parent);
	void CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, const NodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, const NodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, const NodeAnim* pNodeAnim);

	int FindScaling(float AnimationTime, const NodeAnim* pNodeAnim);
	int FindRotation(float AnimationTime, const NodeAnim* pNodeAnim);
	int FindPosition(float AnimationTime, const NodeAnim* pNodeAnim);
};