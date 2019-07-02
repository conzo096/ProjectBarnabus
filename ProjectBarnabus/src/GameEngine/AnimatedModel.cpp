#include "AnimatedModel.h"
#include "Entity.h"
#include "Renderer.h"
#include "NodeAnim.h"

#include <assimp/Importer.hpp>
#include <assimp/PostProcess.h>
#include <assimp/Scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 from)
{
	glm::mat4 to;

	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
	to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

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

AnimatedModel::AnimatedModel(const std::string& fileName) : Model(fileName)
{
	Component("AnimatedModel");
	
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

	for (int i = 0; i < model->mNumMeshes; i++)
	{
		auto mesh = model->mMeshes[i];
		for (int j = 0; j < mesh->mNumBones; j++)
		{
			auto aiBone = mesh->mBones[j];
			Bone bone;
			bone.name = aiBone->mName.C_Str();
			bone.offSet = aiMatrix4x4ToGlm(aiBone->mOffsetMatrix);

		/*	for (int j = 0; j < aiBone->mNumWeights; j++)
			{
				VertexWeight weight;
				weight.vertexId = aiBone->mWeights[j].mVertexId;
				weight.weight = aiBone->mWeights[j].mWeight;
				bone.weights.push_back(weight);
			}*/

			bones.push_back(bone);

			boneMapping[bone.name] = j;
		}
	
	}

	for (int i = 0; i < model->mNumAnimations; i++)
	{
		auto animation = model->mAnimations[i];

		for (int j = 0; j < animation->mNumChannels; j++)
		{
			std::pair<std::string, NodeAnim> pair;

			auto node = animation->mChannels[j];

			pair.first = node->mNodeName.C_Str();
			pair.second.name = node->mNodeName.C_Str();

			for (int key = 0; key < node->mNumPositionKeys; key++)
			{
				PositionKey positionKey;
				positionKey.timeStamp = node->mPositionKeys[key].mTime;
				positionKey.position = aiVec3ToGlm(node->mPositionKeys[key].mValue);
				
				pair.second.positionKeys.push_back(positionKey);
			}

			for (int key = 0; key < node->mNumScalingKeys; key++)
			{
				ScalingKey scalingKey;
				scalingKey.timeStamp = node->mScalingKeys[key].mTime;
				scalingKey.scale = aiVec3ToGlm(node->mScalingKeys[key].mValue);

				pair.second.scalingKeys.push_back(scalingKey);
			}
			
			for (int key = 0; key < node->mNumRotationKeys; key++)
			{
				RotationKey rotationKey;
				rotationKey.timeStamp = node->mRotationKeys[key].mTime;
				rotationKey.rotation = aiQuatToGlm(node->mRotationKeys[key].mValue);

				pair.second.rotationKeys.push_back(rotationKey);
			}

			//boneMapping.insert(pair);
		}
	}

}