#include "BarnabusGameEngine.h"
#include <iostream>
#include <time.h>  

GLFWwindow* BarnabusGameEngine::GetWindow()
{
	return window;
}

void BarnabusGameEngine::SetGame(Game* newGame)
{
	game = newGame;
}

int BarnabusGameEngine::ShouldWindowClose()
{
	return glfwWindowShouldClose(window);
}

bool BarnabusGameEngine::InitialiseGameEngine()
{
	std::cout << "InitialiseGameEngine" << std::endl;
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return false;
	}

	window = glfwCreateWindow(1920, 1080, "Testing", NULL, NULL);

	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	if (!window)
	{
		assert(window != NULL);
		fprintf(stderr, "Window could not be initialised!");
		return false;
	}

	// Set up glew.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "ERROR: %p EXITING!", glewGetErrorString(glewInit()));
		return false;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// V-Sync, does not run without it
	glfwSwapInterval(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

bool BarnabusGameEngine::StartGame()
{
	InitialiseGameEngine();
	game->LoadGameContent();

	while (running)
	{
		float deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
		time += deltaTime * 60;
		lastTime = static_cast<float>(clock());

		running = game->Update(deltaTime);
		game->Render(deltaTime);

		// process events.
		glfwPollEvents();
		
	}
	glfwTerminate();
	return true;
}
