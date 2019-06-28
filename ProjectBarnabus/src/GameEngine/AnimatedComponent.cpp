#include "AnimatedComponent.h"
#include "ModelUtils.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>
AnimatedComponent::AnimatedComponent() : Component("AnimatedComponent")
{
}

AnimatedComponent::~AnimatedComponent()
{
	Component::~Component();
}

void AnimatedComponent::AddVertices(std::vector<float> vertices, std::vector<int> indices)
{

	shader.AddShaderFromFile("res\\Shaders\\Basic.vert",GLShader::VERTEX);
	shader.AddShaderFromFile("res\\Shaders\\Basic.frag",GLShader::FRAGMENT);
	shader.Link();


	verticesCount = vertices.size();

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
}

void AnimatedComponent::Update(double delta)
{
	glm::mat4 identity(1.0);

	double ticksPerSecond = (float)(animation.mTicksPerSecond != 0 ? animation.mTicksPerSecond : 25.0f);
	double timeInTicks = delta * ticksPerSecond;

	// This is most likely wrong - double check.
	double animationTime = timeInTicks / animation.mDuration;

	ReadNodeHeirarchy(animationTime, &root, identity);

	for (short i = 0; i < bones.size(); i++)
	{
		boneTransforms[i] = bones[i].finalTransformation;
	}
}

void AnimatedComponent::Render()
{

	for (int i = 0; i < boneTransforms.size(); i++)
	{
		auto location = shader.GetUniformLocation( "gBones[" + i + ']');
		glUniformMatrix4fv(location, boneTransforms.size(), GL_FALSE, glm::value_ptr(boneTransforms[i]));
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 19 * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 19 * 4, (void*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 19 * 4, (void*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 19 * 4, (void*)32);
	glVertexAttribPointer(4, 4, GL_FLOAT, false, 19 * 4, (void*)44);
	glVertexAttribPointer(5, 4, GL_FLOAT, false, 19 * 4, (void*)60);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}

void AnimatedComponent::ReadNodeHeirarchy(double AnimationTime, aiNode* pNode, glm::mat4 parentTransform)
{
	std::string nodeName = pNode->mName.C_Str();

	glm::mat4 NodeTransformation  = ConvertMat4Matrix(pNode->mTransformation);

	aiNodeAnim pNodeAnim = FindNodeAnim(animation, nodeName);

	glm::vec3 Scaling;
	CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
	glm::mat4 ScalingM = glm::scale (Scaling);

	// Interpolate rotation and generate rotation transformation matrix
	glm::quat RotationQ;
	CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
	glm::mat4 RotationM = glm::toMat4(RotationQ);

	// Interpolate translation and generate translation transformation matrix
	glm::vec3 Translation;
	CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
	glm::mat4 TranslationM = glm::scale(Translation);

	// Combine the above transformations
	NodeTransformation = TranslationM * RotationM * ScalingM;

	glm::mat4 GlobalTransformation = glm::mat4(GetTransform()) * NodeTransformation;

	Bone bone = FindBone(nodeName);

	bone.finalTransformation = globalInverseTransform * (GlobalTransformation * bone.offsetMatrix);

	for (int i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void AnimatedComponent::CalcInterpolatedScaling(glm::vec3 & Out, float AnimationTime, aiNodeAnim pNodeAnim)
{
	if (pNodeAnim.mNumScalingKeys == 1)
	{
		Out = glm::vec3(pNodeAnim.mScalingKeys[0].mValue.x, pNodeAnim.mScalingKeys[0].mValue.y, pNodeAnim.mScalingKeys[0].mValue.z);
		return;
	}

	int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	int NextScalingIndex = (ScalingIndex + 1);
	
	assert(NextScalingIndex < pNodeAnim.mNumScalingKeys);

	float DeltaTime = (float)(pNodeAnim.mScalingKeys[NextScalingIndex].mTime - pNodeAnim.mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim.mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	
	assert(Factor >= 0.0f && Factor <= 1.0f);
	
	glm::vec3 Start = glm::vec3(pNodeAnim.mScalingKeys[ScalingIndex].mValue.x, pNodeAnim.mScalingKeys[ScalingIndex].mValue.y, pNodeAnim.mScalingKeys[ScalingIndex].mValue.z);
	glm::vec3 End = glm::vec3(pNodeAnim.mScalingKeys[NextScalingIndex].mValue.x, pNodeAnim.mScalingKeys[NextScalingIndex].mValue.y, pNodeAnim.mScalingKeys[NextScalingIndex].mValue.z);
	glm::vec3 Delta = End - Start;

	Out = Start + (Delta * Factor);
}

void AnimatedComponent::CalcInterpolatedRotation(glm::quat & Out, float AnimationTime, aiNodeAnim pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim.mNumRotationKeys == 1)
	{
		Out = glm::quat(pNodeAnim.mRotationKeys[0].mValue.w, pNodeAnim.mRotationKeys[0].mValue.x, pNodeAnim.mRotationKeys[0].mValue.y, pNodeAnim.mRotationKeys[0].mValue.z);
		return;
	}

	int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	int NextRotationIndex = (RotationIndex + 1);
	
	assert(NextRotationIndex < pNodeAnim.mNumRotationKeys);

	float DeltaTime = (float)(pNodeAnim.mRotationKeys[NextRotationIndex].mTime - pNodeAnim.mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim.mRotationKeys[RotationIndex].mTime) / DeltaTime;
	
	assert(Factor >= 0.0f && Factor <= 1.0f);
	
	glm::quat StartRotationQ = glm::quat(pNodeAnim.mRotationKeys[0].mValue.w, pNodeAnim.mRotationKeys[0].mValue.x, pNodeAnim.mRotationKeys[0].mValue.y, pNodeAnim.mRotationKeys[0].mValue.z);
	glm::quat EndRotationQ = glm::quat(pNodeAnim.mRotationKeys[NextRotationIndex].mValue.w, pNodeAnim.mRotationKeys[NextRotationIndex].mValue.x, pNodeAnim.mRotationKeys[NextRotationIndex].mValue.y,
											pNodeAnim.mRotationKeys[NextRotationIndex].mValue.z);
	
	Out = glm::slerp(StartRotationQ, EndRotationQ, Factor);
		
}

void AnimatedComponent::CalcInterpolatedPosition(glm::vec3 & Out, float AnimationTime, aiNodeAnim pNodeAnim)
{
	if (pNodeAnim.mNumPositionKeys == 1)
	{
		Out = glm::vec3(pNodeAnim.mPositionKeys[0].mValue.x, pNodeAnim.mPositionKeys[0].mValue.y, pNodeAnim.mPositionKeys[0].mValue.z);
		return;
	}

	int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	int NextPositionIndex = PositionIndex++;
	
	assert(NextPositionIndex < pNodeAnim.mNumPositionKeys);
	
	float DeltaTime = (float)(pNodeAnim.mPositionKeys[NextPositionIndex].mTime - pNodeAnim.mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim.mPositionKeys[PositionIndex].mTime) / DeltaTime;
	
	assert(Factor >= 0.0f && Factor <= 1.0f);
	
	glm::vec3 Start(pNodeAnim.mPositionKeys[PositionIndex].mValue.x, pNodeAnim.mPositionKeys[PositionIndex].mValue.y, pNodeAnim.mPositionKeys[PositionIndex].mValue.z);
	glm::vec3 End(pNodeAnim.mPositionKeys[NextPositionIndex].mValue.x, pNodeAnim.mPositionKeys[NextPositionIndex].mValue.y, pNodeAnim.mPositionKeys[NextPositionIndex].mValue.z);
	glm::vec3 Delta = End - Start;
	Out = Start + (Delta*Factor);
}

int AnimatedComponent::FindScaling(float AnimationTime, aiNodeAnim pNodeAnim)
{
	assert(pNodeAnim.mNumScalingKeys > 0);

	for (int i = 0; i < pNodeAnim.mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim.mScalingKeys[i+1].mTime)
		{
			return i;
		}
	}

	return 0;
}

int AnimatedComponent::FindRotation(float AnimationTime, aiNodeAnim pNodeAnim)
{
	assert(pNodeAnim.mNumRotationKeys > 0);

	for (int i = 0; i < pNodeAnim.mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim.mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

int AnimatedComponent::FindPosition(float AnimationTime, aiNodeAnim pNodeAnim)
{
	for (int i = 0; i < pNodeAnim.mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim.mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

aiNodeAnim AnimatedComponent::FindNodeAnim(aiAnimation pAnimation, std::string NodeName)
{
	for (int i = 0; i < pAnimation.mNumChannels; i++)
	{
		aiNodeAnim pNodeAnim = aiNodeAnim(*pAnimation.mChannels[i]);

		if (pNodeAnim.mNodeName.C_Str() == NodeName)
			return pNodeAnim;
	}

	throw std::exception();
}

Bone AnimatedComponent::FindBone(std::string name)
{
	for (Bone bone : bones)
	{
		if (bone.name == name)
			return bone;
	}

	throw new std::exception();
}

aiNodeAnim FindNodeAnim(aiAnimation pAnimation, std::string NodeName)
{		
	for (int i = 0; i < pAnimation.mNumChannels; i++) 
	{
		aiNodeAnim pNodeAnim = aiNodeAnim(*pAnimation.mChannels[i]);

		if (pNodeAnim.mNodeName.C_Str() == NodeName)
		{
			return pNodeAnim;
		}
	}

	throw std::exception();
}
