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
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(0, 0, 6)*deltaTime);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(0, 0, -6)*deltaTime);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(6, 0, 0)*deltaTime);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(-6, 0, 0)*deltaTime);
		GetParent()->SetPosition(terrain->GetWorldPositionFromGrid(GetParent()->GetPosition()));
	}
}
