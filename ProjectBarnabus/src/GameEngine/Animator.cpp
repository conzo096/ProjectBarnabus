#include "Animator.h"
#include "AnimatedModel.h"

float Animator::GetSpeed()
{
	return speed;
}

void Animator::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Animator::SetState(AnimationState newState)
{
	state = newState;
}

AnimationState Animator::GetState()
{
	return state;
}

std::shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return currentAnimation;
}

void Animator::SetCurrentAnimation(std::shared_ptr<Animation> player)
{
	currentAnimation = player;
}

void Animator::ClearCurrentAnimation()
{
	currentAnimation = NULL;
}

void Animator::Update(float deltaTime)
{
	float delta = deltaTime * speed;

	switch (state)
	{
	case STOP: 
		currentAnimationTime = 0;
		break;
	case PLAY:
		currentAnimationTime += delta;
		break;
	case REWIND:
		currentAnimationTime -= delta;
		if (currentAnimationTime < 0 )
		{
			if (currentAnimation)
			{
				currentAnimationTime = currentAnimation->GetAnimationLength();
			}
		}
		break;
	case PAUSE:
	default:
		break;
	}

}

float Animator::GetAnimationTime()
{
	return currentAnimationTime;
}
