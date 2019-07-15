#pragma once
#include <glm/glm.hpp>
#include <vector>

#define NUM_BONES_PER_VERTEX 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct VertexBoneData
{
	float weights[NUM_BONES_PER_VERTEX];
	int boneIds[NUM_BONES_PER_VERTEX];

	void AddBoneData(unsigned int boneId, float weight)
	{
		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(boneIds); i++)
		{
			if (weights[i] == 0.0)
			{
				boneIds[i] = boneId;
				weights[i] = weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		assert(0);
	}
};

struct Bone
{
	std::string name;
	glm::mat4 offSet;
	glm::mat4 inverseOffSet;

	glm::dmat4 finalTransformation;
};