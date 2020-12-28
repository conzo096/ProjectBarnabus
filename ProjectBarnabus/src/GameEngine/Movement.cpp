#include "Movement.h"
#include "BarnabusGameEngine.h"
#include "AnimatedModel.h"
#include <GLFW/glfw3.h>
Movement::Movement() : Component("Movement")
{
}

Movement::Movement(Terrain* const entityTerrain) : Component("Movement"), terrain(entityTerrain)
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
	auto& animator = GetParent()->GetComponent<AnimatedModel>().GetAnimator();
	animator.SetState(AnimationState::PAUSE);

	if (movement != glm::vec3(0))
	{
		GetParent()->Move(movement);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
		animator.SetState(AnimationState::PLAY);
	}

	movement = glm::vec3(0);
}

void Movement::SetMovement(glm::vec3 move)
{
	movement += move;
}
