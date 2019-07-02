#pragma once
#include "AnimatedModel.h"
#include "Animation.h"

class Animator
{
public:

	Animator();
	~Animator();

private:
	Animation currentAnimation;
	float animationTime = 0;

	AnimatedModel* entity;

};