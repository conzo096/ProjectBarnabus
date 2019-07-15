#pragma once
#include "NodeAnim.h"

class Animation
{
public:
	std::string GetName();
	void SetName(std::string nameToSet);
	float GetTicksPerSecond();
	void SetTicksPerSecond(float ticks);
	float GetAnimationLength();
	void SetAnimationLength(float seconds);

	// todo move this
	std::vector<NodeAnim*> nodes;

private:
	std::string name;
	float ticksPerSecond;
	float animationLength;
};