#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

#define NUM_BONES_PER_VERTEX 4

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

struct VertexBoneData
{

	VertexBoneData()
	{
		ZERO_MEM(weights);
		ZERO_MEM(vertexIds);
	}

	float weights[NUM_BONES_PER_VERTEX];
	int vertexIds[NUM_BONES_PER_VERTEX];

	void AddBoneData(unsigned int boneId, float weight)
	{
		for (unsigned i = 0; i < ARRAY_SIZE_IN_ELEMENTS(vertexIds); i++)
		{
			if (weights[i] == 0.0)
			{
				vertexIds[i] = boneId;
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

	glm::dmat4 finalTransformation;
};