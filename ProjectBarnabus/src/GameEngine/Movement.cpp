#include "Movement.h"
#include "BarnabusGameEngine.h"
#include <GLFW/glfw3.h>
Movement::Movement() : Component("Movement")
{
}

Movement::~Movement()
{
}

void Movement::Update(float deltaTime)
{
	// todo move this to a user movement component
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(0, 0, 6)*deltaTime);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(0, 0, -6)*deltaTime);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(6, 0, 0)*deltaTime);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		GetParent()->Move(glm::vec3(-6, 0, 0)*deltaTime);
	}

}
