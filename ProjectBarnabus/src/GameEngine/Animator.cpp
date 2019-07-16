#include "Animator.h"

float Animator::GetSpeed()
{
	return speed;
}

void Animator::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

std::shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return currentAnimation;
}

void Animator::SetCurrentAnimation(std::shared_ptr<Animation> animation)
{
	currentAnimation = animation;
}

void Animator::Update(double deltaTime)
{
	float delta = deltaTime * speed;

	currentAnimationTime += delta;
}

float Animator::GetAnimationTime()
{
	return currentAnimationTime;
}
