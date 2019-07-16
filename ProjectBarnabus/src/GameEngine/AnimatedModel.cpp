#include "AnimatedModel.h"
#include "Entity.h"
#include "Renderer.h"
#include "NodeAnim.h"

#include <assimp/Importer.hpp>
#include <assimp/PostProcess.h>
#include <assimp/Scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <memory>

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &aiMat)
{
	return {
	aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
	aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
	aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
	aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
	};
}

inline glm::vec3 aiVec3ToGlm(const aiVector3D &from)
{
	return glm::vec3(from.x, from.y, from.z);
}

inline glm::quat aiQuatToGlm(const aiQuaternion &from)
{
	return glm::quat(from.w,from.x,from.y,from.z);
}

AnimatedModel::AnimatedModel(const std::string& fileName) : Model(fileName)
{	
	// Create model importer
	Assimp::Importer loadModel;
	// Read in the model data 
	const aiScene *model = loadModel.ReadFile(fileName, aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices);

	if (!model->HasAnimations())
	{
		return;
	}

	globalInverseTransform = glm::inverse(aiMatrix4x4ToGlm(model->mRootNode->mTransformation));

	for (int i = 0; i < model->mNumMeshes; i++)
	{
		auto mesh = model->mMeshes[i];
		for (int j = 0; j < mesh->mNumBones; j++)
		{
			auto aiBone = mesh->mBones[j];
 			Bone bone;
			bone.name = aiBone->mName.C_Str();
			bone.offSet = aiMatrix4x4ToGlm(aiBone->mOffsetMatrix);
			bone.inverseOffSet = glm::inverse(aiMatrix4x4ToGlm(aiBone->mOffsetMatrix));

			bones.push_back(bone);
			boneMapping[bone.name] = j;

			for (unsigned int k = 0; k < aiBone->mNumWeights; k++)
			{
				unsigned int vertexId = aiBone->mWeights[k].mVertexId;
				float weight = aiBone->mWeights[k].mWeight;
				data[i].InsertBoneDataAt(vertexId,j, weight);
			}
		}	
	}

	for (int i = 0; i < model->mNumAnimations; i++)
	{
		auto animation = model->mAnimations[i];
		std::shared_ptr<Animation> newAnimation = std::make_shared<Animation>();
		newAnimation->SetAnimationLength(animation->mDuration);
		newAnimation->SetName(animation->mName.C_Str());
		newAnimation->SetTicksPerSecond(animation->mTicksPerSecond);

		for (int j = 0; j < animation->mNumChannels; j++)
		{
			auto channel = animation->mChannels[j];
			NodeAnim* node = new NodeAnim;
			node->name = channel->mNodeName.C_Str();
			for (int key = 0; key < channel->mNumPositionKeys; key++)
			{
				PositionKey positionKey;
				positionKey.timeStamp = channel->mPositionKeys[key].mTime;
				positionKey.position = aiVec3ToGlm(channel->mPositionKeys[key].mValue);
				node->positionKeys.push_back(positionKey);
			}

			for (int key = 0; key < channel->mNumScalingKeys; key++)
			{
				ScalingKey scalingKey;
				scalingKey.timeStamp = channel->mScalingKeys[key].mTime;
				scalingKey.scale = aiVec3ToGlm(channel->mScalingKeys[key].mValue);
				node->scalingKeys.push_back(scalingKey);
			}

			for (int key = 0; key < channel->mNumRotationKeys; key++)
			{
				RotationKey rotationKey;
				rotationKey.timeStamp = channel->mRotationKeys[key].mTime;
				rotationKey.rotation = aiQuatToGlm(channel->mRotationKeys[key].mValue);
				node->rotationKeys.push_back(rotationKey);
			}

			newAnimation->nodes.push_back(node);
		}
		animations.push_back(newAnimation);
	}

	LoadNodeTree(rootNode, model->mRootNode, NULL);
	animator.SetCurrentAnimation(animations[0]);
}

void AnimatedModel::Update(double deltaTime)
{
	Model::Update(deltaTime);
	animator.Update(deltaTime);

	for (int i = 0; i < data.size(); i++)
	{
		auto& mesh = data[i];

		mesh.transforms.clear();


		float ticksPerSecond = animator.GetCurrentAnimation()->GetTicksPerSecond() != 0 ? animator.GetCurrentAnimation()->GetTicksPerSecond() : 20.0f;
		float timeInTicks = animator.GetAnimationTime() * ticksPerSecond;
		float animationTime = fmod(timeInTicks, animator.GetCurrentAnimation()->GetAnimationLength());

		ReadNodeHeirarchy(animationTime, rootNode, glm::mat4(1));

		mesh.transforms.resize(bones.size());


		for (unsigned int j = 0; j < bones.size(); j++)
		{
			mesh.transforms[j] = bones[j].finalTransformation;
		}
	}
}

void AnimatedModel::ReadNodeHeirarchy(float animationTime, const Node* node, const glm::mat4 & parentTransform)
{
	std::string nodeName(node->name);

	const std::shared_ptr<Animation> animation = animator.GetCurrentAnimation();

	glm::mat4 nodeTransformation = node->transformation;
	const NodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

	if (nodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		glm::mat4 scaling = glm::scale(CalculateInterpolatedScaling(animationTime, nodeAnim));

		// Interpolate rotation and generate rotation transformation matrix
		glm::mat4 rotation = glm::mat4_cast(CalculateInterpolatedRotation(animationTime, nodeAnim));

		// Interpolate translation and generate translation transformation matrix
		glm::mat4 translation = glm::translate(CalculateInterpolatedPosition(animationTime, nodeAnim));

		// Combine the above transformations
		nodeTransformation = translation * rotation * scaling;
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransformation;

	if (boneMapping.find(nodeName) != boneMapping.end())
	{
		unsigned int BoneIndex = boneMapping[nodeName];
		bones[BoneIndex].finalTransformation = globalInverseTransform * globalTransformation * bones[BoneIndex].offSet;
	}

	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		ReadNodeHeirarchy(animationTime, node->children[i], globalTransformation);
	}

}

const NodeAnim* AnimatedModel::FindNodeAnim(const std::shared_ptr<Animation> animation, const std::string nodeName)
{
	for (int i = 0; i < animation->nodes.size(); i++)
	{
		const NodeAnim* nodeAnim = animation->nodes[i];

		if (std::string(nodeAnim->name) == nodeName)
		{
			return nodeAnim;
		}
	}

	return NULL;
}

void AnimatedModel::LoadNodeTree(Node*& myRootNode, aiNode* rootNode, Node* parent)
{
	myRootNode = new Node;
	myRootNode->transformation = aiMatrix4x4ToGlm(rootNode->mTransformation);
	myRootNode->name = rootNode->mName.C_Str();
	myRootNode->parent = parent;
	myRootNode->children.resize(rootNode->mNumChildren);

	for (int i = 0; i < rootNode->mNumChildren; i++)
	{
		LoadNodeTree(myRootNode->children[i], rootNode->mChildren[i], myRootNode);
	}
}

glm::vec3 AnimatedModel::CalculateInterpolatedScaling(float animationTime, const NodeAnim * nodeAnim)
{
	if (nodeAnim->scalingKeys.size() == 1)
	{
		return nodeAnim->scalingKeys[0].scale;
	}

	unsigned int scalingIndex = FindScaling(animationTime, nodeAnim);
	unsigned int nextScalingIndex = (scalingIndex + 1);

	float deltaTime = nodeAnim->scalingKeys[nextScalingIndex].timeStamp - nodeAnim->scalingKeys[scalingIndex].timeStamp;
	float factor = (animationTime - nodeAnim->scalingKeys[scalingIndex].timeStamp) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const glm::vec3 start = nodeAnim->scalingKeys[scalingIndex].scale;
	const glm::vec3 end = nodeAnim->scalingKeys[nextScalingIndex].scale;
	glm::vec3 delta = end - start;
	return start + factor * delta;
}

glm::quat AnimatedModel::CalculateInterpolatedRotation(float animationTime, const NodeAnim * nodeAnim)
{
	if (nodeAnim->rotationKeys.size() == 1)
	{
		return nodeAnim->rotationKeys[0].rotation;
	}

	unsigned int rotationIndex = FindRotation(animationTime, nodeAnim);
	unsigned int nextRotationIndex = (rotationIndex + 1);
	assert(nextRotationIndex < nodeAnim->rotationKeys.size());
	
	float deltaTime = nodeAnim->rotationKeys[nextRotationIndex].timeStamp - nodeAnim->rotationKeys[rotationIndex].timeStamp;
	float factor = (animationTime - nodeAnim->rotationKeys[rotationIndex].timeStamp) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& startRotationQ = nodeAnim->rotationKeys[rotationIndex].rotation;
	const glm::quat& endRotationQ = nodeAnim->rotationKeys[nextRotationIndex].rotation;
	return glm::normalize(glm::slerp(startRotationQ, endRotationQ, factor));
}

glm::vec3 AnimatedModel::CalculateInterpolatedPosition(float animationTime, const NodeAnim * nodeAnim)
{
	if (nodeAnim->positionKeys.size() == 1)
	{
		return nodeAnim->positionKeys[0].position;
	}

	unsigned int positionIndex = FindPosition(animationTime, nodeAnim);
	unsigned int nextPositionIndex = (positionIndex + 1);
	assert(nextPositionIndex < nodeAnim->positionKeys.size());
	float deltaTime = nodeAnim->positionKeys[nextPositionIndex].timeStamp - nodeAnim->positionKeys[positionIndex].timeStamp;
	float factor = (animationTime - nodeAnim->positionKeys[positionIndex].timeStamp) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3 start = nodeAnim->positionKeys[positionIndex].position;
	const glm::vec3 end = nodeAnim->positionKeys[nextPositionIndex].position;
	glm::vec3 delta = end - start;
	return start + factor * delta;
}

int AnimatedModel::FindScaling(float animationTime, const NodeAnim * nodeAnim)
{
	for (unsigned int i = 0; i < nodeAnim->scalingKeys.size() - 1; i++)
	{
		if (animationTime < nodeAnim->scalingKeys[i + 1].timeStamp)
		{
			return i;
		}
	}

	return 0;
}

int AnimatedModel::FindRotation(float animationTime, const NodeAnim* nodeAnim)
{
	for (unsigned int i = 0; i < nodeAnim->rotationKeys.size() - 1; i++)
	{
		if (animationTime < nodeAnim->rotationKeys[i + 1].timeStamp)
		{
			return i;
		}
	}

	return 0;
}

int AnimatedModel::FindPosition(float animationTime, const NodeAnim* nodeAnim)
{
	for (unsigned int i = 0; i < nodeAnim->positionKeys.size() - 1; i++)
	{
		if (animationTime < nodeAnim->positionKeys[i + 1].timeStamp)
		{
			return i;
		}
	}

	return 0;
}