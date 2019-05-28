#include "BarnabusGameEngine.h"
#include <iostream>

namespace BarnabusGameEngine
{

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

	bool BarnabusGameEngine::LoadGameContent()
	{
		std::cout << "LoadGameContent" << std::endl;
		return true;
	}

	bool BarnabusGameEngine::Update(double deltaTime)
	{
		std::cout << "Update with deltaTime: " << deltaTime << std::endl;
		return true;
	}

	bool BarnabusGameEngine::Render(double deltaTime)
	{
		std::cout << "Render with deltaTime: " << deltaTime << std::endl;
		return true;
	}


	bool BarnabusGameEngine::StartGameEngine()
	{
		if (!InitialiseGameEngine())
		{
			std::cout << "Game engine failed to load!" << std::endl;
		}

		if (!LoadGameContent())
		{
			std::cout << "Game content was not able to be loaded!" << std::endl;
		}

		//todo remove this with a proper condition to exit the loop.
		//todo add proper deltatime value.

		int temporaryCount = 0;
		while (temporaryCount < 10)
		{
			Update(temporaryCount);
			Render(temporaryCount);

			temporaryCount++;
		}

		return true;
	}
}
