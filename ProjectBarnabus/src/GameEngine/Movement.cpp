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
	auto speed = deltaTime * 10;
	// w is foward * amount
	// s is -forward * amount
	// a is left of forward so foward * ?
	// d is right of forward so opposite of above

	auto& animator = GetParent()->GetComponent<AnimatedModel>().GetAnimator();
	animator.SetState(AnimationState::PAUSE);

	glm::vec3 movement(0);
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		movement += dir*speed;
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		movement += -dir * speed;
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		movement += left * speed;
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		movement += -left*speed;
	}

	GetParent()->Move(movement);
	GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));

	if (movement != glm::vec3(0))
	{
		animator.SetState(AnimationState::PLAY);
	}
}
