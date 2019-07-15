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

	void ReadNodeHeirarchy(float animationTime, const Node* pNode, const glm::mat4& parentTransform);

	const NodeAnim* FindNodeAnim(const Animation* pAnimation, const std::string nodeName);


	std::vector<Bone> bones;
	std::map<std::string, int> boneMapping;

	glm::mat4 globalInverseTransform;
	std::vector<Animation*> animations;

	Node* rootNode;
	float totalTime;
private:
	void LoadNodeTree(Node*& myRootNode, aiNode* rootNode,Node* parent);
	glm::vec3 CalcInterpolatedScaling(float animationTime, const NodeAnim* nodeAnim);
	glm::quat CalcInterpolatedRotation(float animationTime, const NodeAnim* nodeAnim);
	glm::vec3 CalcInterpolatedPosition(float animationTime, const NodeAnim* nodeAnim);

	int FindScaling(float animationTime, const NodeAnim* nodeAnim);
	int FindRotation(float animationTime, const NodeAnim* nodeAnim);
	int FindPosition(float animationTime, const NodeAnim* nodeAnim);
};