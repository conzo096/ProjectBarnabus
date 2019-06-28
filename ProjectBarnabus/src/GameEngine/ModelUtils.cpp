#include "ModelUtils.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"
#include "Bone.h"
#include <iostream>
#include <vector>
#include <map> 

glm::mat4 ConvertMat4Matrix(const aiMatrix4x4 &aiMat)
{
	return {
	aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
	aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
	aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
	aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
	};
}

void LoadAnimatedFile(std::string& filePath, std::unique_ptr<AnimatedComponent>& component)
{
	Assimp::Importer loadModel;
	const aiScene* scene = loadModel.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights
	);

	if (scene == NULL || scene->mNumAnimations == 0)
	{
		std::cout << "Failed to load File";
		// not good, FIX
		throw std::exception();
	}

	auto mesh = scene->mMeshes[0];
	int sizeOfVertex = 19;
	int sizeOfVertexUnrigged = 11;

	const int arraySize = mesh->mNumVertices * sizeOfVertex;
	std::vector<float> array(arraySize);
	int index = 0;

	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		aiVector3D position = mesh->mVertices[v];
		aiVector3D normal = mesh->mNormals[v];
		aiVector3D tangent = mesh->mTangents[v];
		aiVector3D texCoord = mesh->mTextureCoords[0][v];
		/**
		 * The above assumes that the program has texture coordinates, if it doesn't the program will throw a null pointer exception.
		 */

		array[index++] = position.x;
		array[index++] = position.y;
		array[index++] = position.z;

		array[index++] = texCoord.x;
		array[index++] = texCoord.y;

		array[index++] = normal.x;
		array[index++] = normal.y;
		array[index++] = normal.z;

		array[index++] = tangent.x;
		array[index++] = tangent.y;
		array[index++] = tangent.z;

		array[index++] = 0;
		array[index++] = 0;
		array[index++] = 0;
		array[index++] = 0;

		array[index++] = 0;
		array[index++] = 0;
		array[index++] = 0;
		array[index++] = 0;
	}
	index = 0;

	std::vector<int> indices = std::vector<int>(mesh->mNumFaces * mesh->mFaces[0].mNumIndices);
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int ind = 0; ind < face.mNumIndices; ind++)
		{
			indices.push_back(face.mIndices[ind]);
		}
	}

	std::map<std::string, int> boneMap;
	std::map<int, int> bone_index_map0;
	std::map<int, int> bone_index_map1;

	for (unsigned int b = 0; b < mesh->mNumBones; b++)
	{
		aiBone bone = aiBone(*mesh->mBones[b]);
		boneMap.insert(std::make_pair(bone.mName.C_Str(), b));

		for (unsigned int w = 0; w < bone.mNumWeights; w++)
		{
			aiVertexWeight weight = bone.mWeights[w];
			int vertexIndex = weight.mVertexId;
			int findex = vertexIndex * sizeOfVertex;

			if (bone_index_map0.find(vertexIndex) != bone_index_map0.end())
			{
				array[(findex + sizeOfVertexUnrigged) + 0] = b;
				array[(findex + sizeOfVertexUnrigged) + 2] = weight.mWeight;
				bone_index_map0.insert(std::make_pair(vertexIndex, 0));
			}
			else if (bone_index_map0[vertexIndex] == 0)
			{
				array[(findex + sizeOfVertexUnrigged) + 1] = b;
				array[(findex + sizeOfVertexUnrigged) + 3] = weight.mWeight;
				bone_index_map0.insert(std::make_pair(vertexIndex, 1));
			}
			else if (bone_index_map1.find(vertexIndex) != bone_index_map1.end())
			{
				array[(findex + sizeOfVertexUnrigged) + 4] = b;
				array[(findex + sizeOfVertexUnrigged) + 6] = weight.mWeight;
				bone_index_map1.insert(std::make_pair(vertexIndex, 0));
			}
			else if (bone_index_map1[vertexIndex] == 0)
			{
				array[(findex + sizeOfVertexUnrigged) + 5] = b;
				array[(findex + sizeOfVertexUnrigged) + 7] = weight.mWeight;
				bone_index_map1.insert(std::make_pair(vertexIndex, 1));
			}
			else
			{
				std::cout << "Max 4 bones per vertex";
				// not good, FIX
				throw std::exception();
			}
		}
	}

	aiMatrix4x4 inverseRootTransform = scene->mRootNode->mTransformation;
		
	glm::mat4	inverseRootTransformation = ConvertMat4Matrix(inverseRootTransform);

	std::vector<Bone> bones = std::vector<Bone>(boneMap.size());

	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		aiBone bone = aiBone(*mesh->mBones[i]);
		bones[i].name = bone.mName.C_Str();
		bones[i].offsetMatrix = ConvertMat4Matrix(bone.mOffsetMatrix);
	}

	std::vector<float> vertices = std::vector<float>(arraySize);

	for (int i = 0; i < arraySize; i++)
	{
		vertices.push_back(array.at(i));
	}

	std::reverse(std::begin(vertices), std::end(vertices));
	std::reverse(std::begin(indices), std::end(indices));

	component->AddVertices(vertices, indices);

	component->scene = scene;
	component->animation = aiAnimation(*scene->mAnimations[0]);
	//component->bones = bones;
	//component->boneTransforms = std::vector<glm::mat4>(bones.size());
	//// Need to copy this?
	//component->root = scene->mRootNode;
	//component->globalInverseTransform = inverseRootTransformation;

 }
