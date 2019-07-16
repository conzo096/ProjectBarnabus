#pragma once
#include <memory>
#include <vector>

class Animation;

enum AnimationState
{
	PLAY,
	PAUSE,
	STOP,
	REWIND
};
class Animator
{
public:

	float GetSpeed();
	void SetSpeed(float newSpeed);

	std::shared_ptr<Animation> GetCurrentAnimation();
	void SetCurrentAnimation(std::shared_ptr<Animation>animation);
	void ClearCurrentAnimation();
	void Update(double deltaTime);
	float GetAnimationTime();
private:
	float speed = 1;
	float currentAnimationTime;

	AnimationState state;

	std::shared_ptr<Animation> currentAnimation;
};