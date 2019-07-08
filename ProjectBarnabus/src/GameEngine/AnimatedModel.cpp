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

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 from)
{
	glm::mat4 to;

	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
	to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

	return to;
}

inline glm::mat4 FloatToGlm(const float from)
{
	glm::mat4 to;

	to[0][0] = (GLfloat)from; to[0][1] = (GLfloat)from;  to[0][2] = (GLfloat)from; to[0][3] = (GLfloat)from;
	to[1][0] = (GLfloat)from; to[1][1] = (GLfloat)from;  to[1][2] = (GLfloat)from; to[1][3] = (GLfloat)from;
	to[2][0] = (GLfloat)from; to[2][1] = (GLfloat)from;  to[2][2] = (GLfloat)from; to[2][3] = (GLfloat)from;
	to[3][0] = (GLfloat)from; to[3][1] = (GLfloat)from;  to[3][2] = (GLfloat)from; to[3][3] = (GLfloat)from;

	return to;
}

inline glm::vec3 aiVec3ToGlm(const aiVector3D from)
{
	return glm::vec3(from.x, from.y, from.z);
}

inline glm::quat aiQuatToGlm(const aiQuaternion from)
{
	return glm::quat(from.x, from.y, from.z, from.w);
}

inline glm::mat3 aiMatrix3x3ToGlm(const aiMatrix3x3 &from)
{
	glm::mat3 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;
	return to;
}

AnimatedModel::AnimatedModel(const std::string& fileName) : Model(fileName)
{	
	// Create model importer
	Assimp::Importer loadModel;
	// Read in the model data 
	const aiScene *model = loadModel.ReadFile(fileName, aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_ValidateDataStructure
		| aiProcess_FindInvalidData
		| aiProcess_FixInfacingNormals
		| aiProcess_ImproveCacheLocality
		| aiProcess_GenUVCoords);

	if (!model->HasAnimations())
	{
		return;
	}

	m_GlobalInverseTransform = glm::inverse(aiMatrix4x4ToGlm(model->mRootNode->mTransformation));
	// This approach is limited to only having one mesh in the model, otherwise vertexids need to be 
	// recalculated.

	for (int i = 0; i < model->mNumMeshes; i++)
	{
		auto mesh = model->mMeshes[i];
		for (int j = 0; j < mesh->mNumBones; j++)
		{
			auto aiBone = mesh->mBones[j];
 			Bone bone;
			bone.name = aiBone->mName.C_Str();
			bone.offSet = glm::inverse(aiMatrix4x4ToGlm(aiBone->mOffsetMatrix));
	
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
		Animation* newAnimation = new Animation;
		newAnimation->animationLength = animation->mDuration;
		newAnimation->name = animation->mName.C_Str();
		newAnimation->ticksPerSecond = animation->mTicksPerSecond;

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
			}

			newAnimation->keyframes.push_back(node);
		}
		animations.push_back(newAnimation);
	}

	LoadNodeTree(rootNode, model->mRootNode, NULL);
}

void AnimatedModel::Update(double deltaTime)
{
	Model::Update(deltaTime);

	for (int i = 0; i < data.size(); i++)
	{
		auto mesh = data[i];
		totalTime += deltaTime;
		//if (totalTime > animations[0]->animationLength)
		totalTime = 0;
		mesh.transforms.clear();

		float TicksPerSecond = animations[0]->ticksPerSecond != 0 ? animations[0]->ticksPerSecond : 20.0f;
		float TimeInTicks = totalTime * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, animations[0]->animationLength);

		ReadNodeHeirarchy(AnimationTime, rootNode, glm::mat4(1));

		mesh.transforms.resize(bones.size());


		for (unsigned int j = 0; j < bones.size(); j++)
		{
			mesh.transforms[j] = bones[j].finalTransformation;
		}
	}
}

void AnimatedModel::ReadNodeHeirarchy(float AnimationTime, const Node* pNode, const glm::mat4 & ParentTransform)
{
	std::string NodeName(pNode->name);

	const Animation* animation = animations[0];

	glm::mat4 NodeTransformation(pNode->transformation);
	const NodeAnim* pNodeAnim = FindNodeAnim(animation, NodeName);

	if (pNodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		glm::vec3 Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM(glm::scale(glm::mat4(1),Scaling));

		// Interpolate rotation and generate rotation transformation matrix
		glm::quat RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM = glm::mat4(1);

		// Interpolate translation and generate translation transformation matrix
		glm::vec3 Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(glm::mat4(1),Translation);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (boneMapping.find(NodeName) != boneMapping.end())
	{
		unsigned int BoneIndex = boneMapping[NodeName];
		bones[BoneIndex].finalTransformation = m_GlobalInverseTransform * GlobalTransformation * bones[BoneIndex].offSet;
	}

	for (unsigned int i = 0; i < pNode->children.size(); i++)
	{
		ReadNodeHeirarchy(AnimationTime, pNode->children[i], GlobalTransformation);
	}

}

const NodeAnim* AnimatedModel::FindNodeAnim(const Animation* animation, const std::string NodeName)
{
	for (int i = 0; i < animation->keyframes.size(); i++)
	{
		const NodeAnim* pNodeAnim = animation->keyframes[i];

		if (std::string(pNodeAnim->name) == NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

void AnimatedModel::LoadNodeTree(Node*& myRootNode, aiNode* rootNode, Node* parent)
{
	myRootNode = new Node;
	myRootNode->transformation = aiMatrix4x4ToGlm(rootNode->mTransformation);
	myRootNode->name = rootNode->mName.C_Str();

	myRootNode->children.resize(rootNode->mNumChildren);

	for (int i = 0; i < rootNode->mNumChildren; i++)
	{
		LoadNodeTree(myRootNode->children[i], rootNode->mChildren[i], myRootNode);
	}
}

void AnimatedModel::CalcInterpolatedScaling(glm::vec3 & Out, float AnimationTime, const NodeAnim * pNodeAnim)
{
	if (pNodeAnim->scalingKeys.size() == 0)
	{
		Out = glm::vec3(1.0);
		return;
	}

	if (pNodeAnim->scalingKeys.size() == 1)
	{
		Out = pNodeAnim->scalingKeys[0].scale;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);

	float DeltaTime = (float)(pNodeAnim->scalingKeys[NextScalingIndex].timeStamp - pNodeAnim->scalingKeys[ScalingIndex].timeStamp);
	float Factor = (AnimationTime - (float)pNodeAnim->scalingKeys[ScalingIndex].timeStamp) / DeltaTime;

	assert(Factor >= 0.0f && Factor <= 1.0f);

	const glm::vec3 Start = pNodeAnim->scalingKeys[ScalingIndex].scale;
	const glm::vec3 End = pNodeAnim->scalingKeys[NextScalingIndex].scale;
	glm::vec3 Delta = End - Start;
	Out = Start + Factor * Delta;

}

void AnimatedModel::CalcInterpolatedRotation(glm::quat & Out, float AnimationTime, const NodeAnim * pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->rotationKeys.size() == 0)
	{
		Out = glm::quat(1.0,glm::vec3(1.0));
		return;
	}

	if (pNodeAnim->rotationKeys.size() == 1)
	{
		Out = pNodeAnim->rotationKeys[0].rotation;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->rotationKeys.size());
	
	float DeltaTime = (float)(pNodeAnim->rotationKeys[NextRotationIndex].timeStamp - pNodeAnim->rotationKeys[RotationIndex].timeStamp);
	float Factor = (AnimationTime - (float)pNodeAnim->rotationKeys[RotationIndex].timeStamp) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const glm::quat& StartRotationQ = pNodeAnim->rotationKeys[RotationIndex].rotation;
	const glm::quat& EndRotationQ = pNodeAnim->rotationKeys[NextRotationIndex].rotation;
	Out = glm::lerp(StartRotationQ, EndRotationQ, Factor);
	Out = glm::normalize(Out);

}

void AnimatedModel::CalcInterpolatedPosition(glm::vec3 & Out, float AnimationTime, const NodeAnim * pNodeAnim)
{

	if (pNodeAnim->positionKeys.size() == 0)
	{
		Out = glm::vec3(1.0);
		return;
	}

	if (pNodeAnim->positionKeys.size() == 1)
	{
		Out = pNodeAnim->positionKeys[0].position;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->positionKeys.size());
	float DeltaTime = (float)(pNodeAnim->positionKeys[NextPositionIndex].timeStamp - pNodeAnim->positionKeys[PositionIndex].timeStamp);
	float Factor = (AnimationTime - (float)pNodeAnim->positionKeys[PositionIndex].timeStamp) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const glm::vec3 Start = pNodeAnim->positionKeys[PositionIndex].position;
	const glm::vec3 End = pNodeAnim->positionKeys[NextPositionIndex].position;
	glm::vec3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

int AnimatedModel::FindScaling(float AnimationTime, const NodeAnim * pNodeAnim)
{
	//assert(pNodeAnim->scalingKeys.size() > 0);

	for (unsigned int i = 0; i < pNodeAnim->scalingKeys.size() - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->scalingKeys[i + 1].timeStamp)
		{
			return i;
		}
	}

	//assert(0);

	return 0;
}

int AnimatedModel::FindRotation(float AnimationTime, const NodeAnim* pNodeAnim)
{
	//assert(pNodeAnim->rotationKeys.size() > 0);

	for (unsigned int i = 0; i < pNodeAnim->rotationKeys.size() - 1; i++)
	{
		if (AnimationTime < pNodeAnim->rotationKeys[i + 1].timeStamp) {
			return i;
		}
	}

	//assert(0);

	return 0;
}

int AnimatedModel::FindPosition(float AnimationTime, const NodeAnim* pNodeAnim)
{
	//assert(pNodeAnim->positionKeys.size() > 0);

	for (unsigned int i = 0; i < pNodeAnim->positionKeys.size() - 1; i++)
	{
		if (AnimationTime < pNodeAnim->positionKeys[i + 1].timeStamp) {
			return i;
		}
	}

	//assert(0);

	return 0;
}