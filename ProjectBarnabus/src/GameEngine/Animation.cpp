#include "Animation.h"

std::string Animation::GetName()
{
	return name;
}

void Animation::SetName(std::string nameToSet)
{
	name = nameToSet;
}

float Animation::GetTicksPerSecond()
{
	return ticksPerSecond;
}

void Animation::SetTicksPerSecond(float ticks)
{
	ticksPerSecond = ticks;
}

float Animation::GetAnimationLength()
{
	return animationLength;
}

void Animation::SetAnimationLength(float seconds)
{
	animationLength = seconds;
}
