#pragma once
#include "Entity.h"
#include "Bone.h"
#include "GlShader.h"

#include <vector>
#include <assimp/scene.h>

class AnimatedComponent : public Component
{
public:
	AnimatedComponent();
	~AnimatedComponent();

	void AddVertices(std::vector<float> vertices, std::vector<int> indices);

	void Update(double delta) override;
	void Render() override;

private:
	void ReadNodeHeirarchy(double AnimationTime, aiNode* pNode, glm::mat4 parentTransform);
	void CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, aiNodeAnim pNodeAnim);
	void CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, aiNodeAnim pNodeAnim);
	void CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, aiNodeAnim pNodeAnim);

	int FindScaling(float AnimationTime, aiNodeAnim pNodeAnim);
	int FindRotation(float AnimationTime, aiNodeAnim pNodeAnim);
	int FindPosition(float AnimationTime, aiNodeAnim pNodeAnim);

	aiNodeAnim FindNodeAnim(aiAnimation pAnimation, std::string NodeName);
	Bone FindBone(std::string name);

public:
	glm::mat4 globalInverseTransform;
	std::vector<Bone> bones;
	std::vector<glm::mat4> boneTransforms;

	 aiNode root;
	 aiAnimation animation;
	 const aiScene* scene;

	 GLShader shader;
private:

	unsigned int vbo;
	unsigned int ibo;
	unsigned int verticesCount;
};