#include "Movement.h"
#include "BarnabusGameEngine.h"
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
	glm::vec3 dir = glm::normalize(camera->GetTarget() - camera->GetPosition());
	auto speed = deltaTime * 6;

	// w is foward * amount
	// s is -forward * amount
	// a is left of forward so foward * ?
	// d is right of forward so opposite of above


	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		GetParent()->Move(dir*speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		GetParent()->Move(-dir*speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		GetParent()->Move(glm::normalize(glm::vec3(1, 0, 0) * dir ) * speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		GetParent()->Move(glm::normalize(glm::vec3(-1, 0, 0) * dir) * speed);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
}
