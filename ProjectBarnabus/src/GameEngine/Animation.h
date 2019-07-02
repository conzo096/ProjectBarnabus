#pragma once
#include "KeyFrame.h"

#include <vector>

class Animation
{
public:

	std::vector<KeyFrame> keyframes;
	float animationLength;
};