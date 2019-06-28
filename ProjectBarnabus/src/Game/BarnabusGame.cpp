#include <time.h>  
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include "BarnabusGame.h"
#include "../GameEngine/BarnabusGameEngine.h"
#include "../GameEngine/FreeCamera.h"
#include "../GameEngine/AnimatedComponent.h"
#include "../GameEngine/ModelUtils.h"
BarnabusGame::BarnabusGame()
{

}

BarnabusGame::~BarnabusGame()
{

}

bool BarnabusGame::LoadGameContent()
{
	auto cameraComponent = std::make_unique<FreeCamera>(70);
	camera.AddComponent(std::move(cameraComponent));

	std::string fileName("res\\Models\\AnimatedModels\\model.dae");
	auto animationComponent = std::make_unique<AnimatedComponent>();
	LoadAnimatedFile(fileName, animationComponent);
	animation.AddComponent(std::move(animationComponent));

	return true;
}

bool BarnabusGame::Update(double deltaTime)
{
	camera.Update(deltaTime);
	animation.Update(deltaTime);
	// Close the window if it has been asked too.
	if (BarnabusGameEngine::Get().ShouldWindowClose() || glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	else
	{
		return true;
	}

	return false;
}

bool BarnabusGame::Render(double deltaTime)
{
	glClearColor(0.1f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	animation.Render();

	glfwSwapBuffers(BarnabusGameEngine::Get().GetWindow());
		return true;
}