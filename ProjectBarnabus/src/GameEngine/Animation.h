#pragma once
#include "KeyFrame.h"

#include <vector>

class Animation
{
public:

	std::string name;
	float ticksPerSecond;
	std::vector<NodeAnim*> keyframes;
	float animationLength;
};