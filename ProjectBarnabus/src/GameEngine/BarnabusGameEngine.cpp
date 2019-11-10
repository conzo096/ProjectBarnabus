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

void BarnabusGameEngine::SetPriority(StringLog::Priority priority)
{
	messagePriority = priority;
}

void BarnabusGameEngine::AddMessageLog(StringLog & log)
{
	logs.push_back(log);
}

void BarnabusGameEngine::PrintLogs()
{
	for(int i =0; i < logs.size(); i++)
	{
		auto& log = logs.at(i);
		if (log.GetPriority() > messagePriority)
		{
			std::cout << log.GetMessage().c_str() << std::endl;
		}
	}
	logs.clear();
}

bool BarnabusGameEngine::InitialiseGameEngine()
{
	AddMessageLog(StringLog("Initialising Game Engine", StringLog::Priority::Low));
	if (!glfwInit())
	{
		AddMessageLog(StringLog("ERROR: glfw failed init! exiting.", StringLog::Priority::Critical));
		return false;
	}

	window = glfwCreateWindow(1920, 1080, "Testing", NULL, NULL);

	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	if (!window)
	{
		assert(window != NULL);
		AddMessageLog(StringLog("Error: Window could not be initialised!", StringLog::Priority::Critical));
		return false;
	}

	// Set up glew.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		AddMessageLog(StringLog("ERROR: glew failed init!exiting.", StringLog::Priority::Critical));
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

	AddMessageLog(StringLog("Game engine initialiation successful", StringLog::Priority::Low));
	return true;
}

bool BarnabusGameEngine::StartGame()
{
	InitialiseGameEngine();
	PrintLogs();
	game->LoadGameContent();
	PrintLogs();
	while (running)
	{
		float deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
		time += deltaTime * 60;
		lastTime = static_cast<float>(clock());

		running = game->Update(deltaTime);
		PrintLogs();
		game->Render(deltaTime);
		PrintLogs();
		// process events.
		glfwPollEvents();
		
	}
	glfwTerminate();
	return true;
}
