#include "Movement.h"
#include "BarnabusGameEngine.h"
#include "AnimatedModel.h"
#include <GLFW/glfw3.h>
Movement::Movement() : Component("Movement")
{
}

Movement::Movement(Terrain* const entityTerrain, Camera* const entityCamera) : Component("Movement"), terrain(entityTerrain), camera(entityCamera)
{

}

Movement::~Movement()
{
}

void Movement::SetTerrain(Terrain* const entityTerrain)
{
	terrain = entityTerrain;
}

void Movement::Update(float deltaTime)
{
	glm::vec3 up = camera->GetOrientation();
	glm::vec3 dir = glm::normalize(camera->GetTarget() - camera->GetPosition());
	dir.y = 0;
	glm::vec3 left = glm::normalize(glm::cross(up, dir));
	left.y = 0;
	auto speed = deltaTime * 30;
	// w is foward * amount
	// s is -forward * amount
	// a is left of forward so foward * ?
	// d is right of forward so opposite of above

	auto& animator = GetParent()->GetComponent<AnimatedModel>().GetAnimator();
	animator.SetState(AnimationState::PAUSE);

	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		GetParent()->Move(dir*speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
		animator.SetState(AnimationState::PLAY);

	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		GetParent()->Move(-dir*speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
		animator.SetState(AnimationState::PLAY);

	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		GetParent()->Move(left*speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
		animator.SetState(AnimationState::PLAY);

	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		GetParent()->Move(-left*speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
		animator.SetState(AnimationState::PLAY);

	}
}
