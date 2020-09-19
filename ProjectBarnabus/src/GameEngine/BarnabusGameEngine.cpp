#include "BarnabusGameEngine.h"
#include <iostream>
#include <time.h>  

GLFWwindow* BarnabusGameEngine::GetWindow()
{
	return renderer ? renderer->GetWindow() : nullptr;
}

void BarnabusGameEngine::SetGame(std::unique_ptr<IGame> newGame)
{
	game = std::move(newGame);
}

int BarnabusGameEngine::ShouldWindowClose()
{
	return glfwWindowShouldClose(renderer ? renderer->GetWindow() : nullptr);
}

void BarnabusGameEngine::SetPriority(StringLog::Priority priority)
{
	messagePriority = priority;
}

void BarnabusGameEngine::AddMessageLog(StringLog log)
{
	logs.push_back(log);
}

bool BarnabusGameEngine::AddShader(std::string name, std::unique_ptr<GLShader> shader)
{
	auto result = shaders.insert(std::pair < std::string, std::unique_ptr<GLShader>>(name, std::move(shader)));
	return result.second;
}

GLShader* BarnabusGameEngine::GetShader(std::string name)
{
	auto it = shaders.find(name);
	assert(it != shaders.end());

	return it->second.get();
}

bool BarnabusGameEngine::AddTexture(std::string name, std::unique_ptr<Texture> texture)
{
	auto result = textures.insert(std::pair < std::string, std::unique_ptr<Texture>>(name, std::move(texture)));
	return result.second;
}

bool BarnabusGameEngine::HasTexture(std::string name)
{
	return textures.find(name) != textures.end();
}

Texture * BarnabusGameEngine::GetTexture(std::string name)
{
	auto it = textures.find(name);
	assert(it != textures.end());

	return it->second.get();
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

	if (!renderer)
	{
		AddMessageLog(StringLog("Renderer not assigned", StringLog::Priority::Critical));
		return false;
	}

	auto engineRenderResult = renderer->InitialiseGameEngine();
	return engineRenderResult;
}

bool BarnabusGameEngine::StartGame(std::unique_ptr<IRenderer> renderEngine)
{
	renderer = std::move(renderEngine);

	InitialiseGameEngine();
	PrintLogs();
	game->LoadGameContent();
	PrintLogs();
	
	while (running)
	{
		float deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
		time += deltaTime;
		lastTime = static_cast<float>(clock());

		running = game->Update(deltaTime);
		PrintLogs();
		game->Render(deltaTime);
		PrintLogs();
		// process events.
		glfwPollEvents();
		
	}
	AddMessageLog(StringLog("Game now exiting", StringLog::Priority::Low));
	PrintLogs();
	glfwTerminate();
	game.reset();
	PrintLogs();
	return true;
}
