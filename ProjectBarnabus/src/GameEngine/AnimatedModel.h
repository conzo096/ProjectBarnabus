#pragma once
#include "Model.h"
#include "Node.h"
#include "NodeAnim.h"
#include "Animation.h"
#include "Animator.h"
#include <map>

class AnimatedModel : public Model
{
public:
	AnimatedModel(const std::string& fileName);

	void SetAnimation(std::string animationName);
	Animator& GetAnimator();
public:
	void Update(float deltaTime) override;
private:

	void ReadNodeHeirarchy(float animationTime, const Node* pNode, const glm::mat4& parentTransform);
	const NodeAnim* FindNodeAnim(const std::shared_ptr<Animation> pAnimation, const std::string nodeName);

	glm::vec3 CalculateInterpolatedScaling(float animationTime, const NodeAnim* nodeAnim);
	glm::quat CalculateInterpolatedRotation(float animationTime, const NodeAnim* nodeAnim);
	glm::vec3 CalculateInterpolatedPosition(float animationTime, const NodeAnim* nodeAnim);

	int FindScaling(float animationTime, const NodeAnim* nodeAnim);
	int FindRotation(float animationTime, const NodeAnim* nodeAnim);
	int FindPosition(float animationTime, const NodeAnim* nodeAnim);

private:
	void UpdateNodeMeshes(Node*& rootNode, float deltaTime);
private:
	std::vector<Bone> bones;
	std::map<std::string, int> boneMapping;

	glm::mat4 globalInverseTransform;
	std::map<std::string, std::shared_ptr<Animation>> animations;

	Animator animator;
};