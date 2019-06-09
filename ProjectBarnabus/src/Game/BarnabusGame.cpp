#include <time.h>  
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include "BarnabusGame.h"
#include "../GameEngine/BarnabusGameEngine.h"

BarnabusGame::BarnabusGame()
{

}

BarnabusGame::~BarnabusGame()
{

}

bool BarnabusGame::LoadGameContent()
{
	return true;
}

bool BarnabusGame::Update(double deltaTime)
{
	// process events.
	glfwPollEvents();
	// Close the window if it has been asked too.
	if (glfwWindowShouldClose(BarnabusGameEngine::Get().window))
		return false;
	else
		return true;
}

bool BarnabusGame::Render(double deltaTime)
{
	glClearColor(0.1f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glfwSwapBuffers(BarnabusGameEngine::Get().window);
		return true;
}