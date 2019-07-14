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

	// Not implemented.
	Node* rootNode;
	float totalTime;
private:
	void LoadNodeTree(Node*& myRootNode, aiNode* rootNode,Node* parent);
	void CalcInterpolatedScaling(glm::vec3& out, float animationTime, const NodeAnim* nodeAnim);
	void CalcInterpolatedRotation(glm::quat& out, float animationTime, const NodeAnim* nodeAnim);
	void CalcInterpolatedPosition(glm::vec3& out, float animationTime, const NodeAnim* nodeAnim);

	int FindScaling(float animationTime, const NodeAnim* nodeAnim);
	int FindRotation(float animationTime, const NodeAnim* nodeAnim);
	int FindPosition(float animationTime, const NodeAnim* nodeAnim);
};