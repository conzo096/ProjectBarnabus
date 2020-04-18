#include "AnimatedModel.h"
#include "Entity.h"
#include "Renderer.h"
#include "NodeAnim.h"
#include "BarnabusGameEngine.h"

#include <assimp/Importer.hpp>
#include <assimp/PostProcess.h>
#include <assimp/Scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <memory>

namespace
{
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
		return glm::quat(from.w, from.x, from.y, from.z);
	}

	void LoadNodeTree(Node*& myRootNode, aiNode* rootNode, Node* parent)
	{
		myRootNode = new Node;
		myRootNode->transformation = aiMatrix4x4ToGlm(rootNode->mTransformation);
		myRootNode->name = rootNode->mName.C_Str();
		myRootNode->parent = parent;
		myRootNode->children.resize(rootNode->mNumChildren);

		for (unsigned int i = 0; i < rootNode->mNumChildren; i++)
		{
			LoadNodeTree(myRootNode->children[i], rootNode->mChildren[i], myRootNode);
		}
	}

	void LoadBoneData(MeshNode*& meshRootNode, aiNode* rootNode, const aiScene* scene, std::vector<Bone>& bones, std::map<std::string, int>& boneMapping)
	{
		for (unsigned int i = 0; i < rootNode->mNumMeshes; i++)
		{
			auto mesh = scene->mMeshes[rootNode->mMeshes[i]];
			for (unsigned int j = 0; j < mesh->mNumBones; j++)
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
					meshRootNode->data[i].InsertBoneDataAt(vertexId, j, weight);
				}
			}
		}

		for (int i = 0; i < meshRootNode->children.size(); i++)
		{
			LoadBoneData(meshRootNode->children[i], rootNode->mChildren[i], scene, bones, boneMapping);
		}
	}
}

AnimatedModel::AnimatedModel(const std::string& fileName) : Model(fileName)
{	
	token = "Animated Model";

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

	LoadBoneData(rootMeshNode, model->mRootNode, model, bones, boneMapping);

	for (unsigned int i = 0; i < model->mNumAnimations; i++)
	{
		auto player = model->mAnimations[i];
		std::shared_ptr<Animation> newAnimation = std::make_shared<Animation>();
		newAnimation->SetAnimationLength(player->mDuration);
		newAnimation->SetName(player->mName.C_Str());
		newAnimation->SetTicksPerSecond(player->mTicksPerSecond);

		for (unsigned int j = 0; j < player->mNumChannels; j++)
		{
			auto channel = player->mChannels[j];
			NodeAnim* node = new NodeAnim;
			node->name = channel->mNodeName.C_Str();
			for (unsigned int key = 0; key < channel->mNumPositionKeys; key++)
			{
				PositionKey positionKey;
				positionKey.timeStamp = channel->mPositionKeys[key].mTime;
				positionKey.position = aiVec3ToGlm(channel->mPositionKeys[key].mValue);
				node->positionKeys.push_back(positionKey);
			}

			for (unsigned int key = 0; key < channel->mNumScalingKeys; key++)
			{
				ScalingKey scalingKey;
				scalingKey.timeStamp = channel->mScalingKeys[key].mTime;
				scalingKey.scale = aiVec3ToGlm(channel->mScalingKeys[key].mValue);
				node->scalingKeys.push_back(scalingKey);
			}

			for (unsigned int key = 0; key < channel->mNumRotationKeys; key++)
			{
				RotationKey rotationKey;
				rotationKey.timeStamp = channel->mRotationKeys[key].mTime;
				rotationKey.rotation = aiQuatToGlm(channel->mRotationKeys[key].mValue);
				node->rotationKeys.push_back(rotationKey);
			}

			newAnimation->nodes.push_back(node);
		}
		animations.insert(std::pair<std::string, std::shared_ptr<Animation>>(newAnimation->GetName(),newAnimation));
	}

	LoadNodeTree(rootNode, model->mRootNode, NULL);
}

void AnimatedModel::Update(float deltaTime)
{
	Model::Update(deltaTime);

	animator.Update(deltaTime);

	UpdateNodeMeshes(rootMeshNode, deltaTime);
}

void AnimatedModel::SetAnimation(std::string animationName)
{
	const auto player = animations.find(animationName);
	if (player == animations.end())
	{
		assert(player != animations.end());
		BarnabusGameEngine::Get().AddMessageLog(
			StringLog("Animation not found with key: " + animationName, StringLog::Priority::Critical));
	}

	animator.SetCurrentAnimation(player->second);
}

void AnimatedModel::ReadNodeHeirarchy(float animationTime, const Node* node, const glm::mat4 & parentTransform)
{
	std::string nodeName(node->name);

	const std::shared_ptr<Animation> player = animator.GetCurrentAnimation();

	glm::mat4 nodeTransformation = node->transformation;
	const NodeAnim* nodeAnim = FindNodeAnim(player, nodeName);

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

const NodeAnim* AnimatedModel::FindNodeAnim(const std::shared_ptr<Animation> player, const std::string nodeName)
{
	for (int i = 0; i < player->nodes.size(); i++)
	{
		const NodeAnim* nodeAnim = player->nodes[i];

		if (std::string(nodeAnim->name) == nodeName)
		{
			return nodeAnim;
		}
	}

	return NULL;
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

void AnimatedModel::UpdateNodeMeshes(MeshNode*& rootMeshNode, float deltaTime)
{
	for (auto& mesh : rootMeshNode->data)
	{
		mesh.transforms.clear();

		float ticksPerSecond = animator.GetCurrentAnimation()->GetTicksPerSecond() != 0 ? animator.GetCurrentAnimation()->GetTicksPerSecond() : 20.0f;
		float timeInTicks = animator.GetAnimationTime() * ticksPerSecond;
		float animationTime = fmod(timeInTicks, animator.GetCurrentAnimation()->GetAnimationLength());

		ReadNodeHeirarchy(animationTime, rootNode, glm::mat4(1));

		mesh.transforms.resize(bones.size());

		if (!animator.GetCurrentAnimation())
		{
			for (unsigned int j = 0; j < bones.size(); j++)
			{
				mesh.transforms[j] = glm::mat4(1);
			}
		}
		else
		{
			for (unsigned int j = 0; j < bones.size(); j++)
			{
				mesh.transforms[j] = bones[j].finalTransformation;
			}
		}
	}

	for (auto& child : rootMeshNode->children)
	{
		UpdateNodeMeshes(child, deltaTime);
	}
}
