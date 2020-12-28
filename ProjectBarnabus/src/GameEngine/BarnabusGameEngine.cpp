#include "BarnabusGameEngine.h"
#include <iostream>
#include <time.h>  

namespace
{
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		BarnabusGameEngine::Get().GetKeyCallback()(key, action);
	}
}

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
	return renderer ? renderer->ShouldWindowClose() : true;
}

void BarnabusGameEngine::SetPriority(StringLog::Priority priority)
{
	messagePriority = priority;
}

void BarnabusGameEngine::AddMessageLog(StringLog log)
{
	logs.push_back(log);
}

bool BarnabusGameEngine::AddShader(std::string name, std::unique_ptr<IShader> shader)
{
	return renderer->AddShader(name, std::move(shader));
}

IShader* BarnabusGameEngine::GetShader(std::string name)
{
	return renderer->GetShader(name);
}

bool BarnabusGameEngine::AddTexture(std::string name, std::unique_ptr<ITexture> texture)
{
	auto result = textures.insert(std::pair < std::string, std::unique_ptr<ITexture>>(name, std::move(texture)));
	return result.second;
}

bool BarnabusGameEngine::HasTexture(std::string name)
{
	return textures.find(name) != textures.end();
}

ITexture * BarnabusGameEngine::GetTexture(std::string name)
{
	auto it = textures.find(name);
	assert(it != textures.end());

	return it->second.get();
}

IRenderer::GraphicsRenderer BarnabusGameEngine::GetRenderType()
{
	return renderer->GetRenderType();
}

IRenderer* BarnabusGameEngine::GetRenderer()
{
	return renderer.get();
}

void BarnabusGameEngine::InitialiseMesh(MeshData & data)
{
	renderer->InitialiseMesh(data);
}

void BarnabusGameEngine::UpdateBaseVertexBuffers(MeshData & data)
{
	renderer->UpdateBaseVertexBuffers(data);
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

	return renderer->InitialiseGameEngine();
}

bool BarnabusGameEngine::StartGame(std::unique_ptr<IRenderer> renderEngine)
{
	renderer = std::move(renderEngine);

	auto engineRenderResult = InitialiseGameEngine();

	if (engineRenderResult)
	{
		PrintLogs();
		if (game)
		{
			game->LoadGameContent();
			PrintLogs();

			while (running)
			{
				float deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
				time += deltaTime;
				lastTime = static_cast<float>(clock());

				// process events.
				glfwPollEvents();
				running = game->Update(deltaTime);
				PrintLogs();
				game->Render(deltaTime);
				PrintLogs();
			}
		}
		else
		{
			AddMessageLog(StringLog("No game set up. Exiting", StringLog::Priority::Critical));
		}
	}
	else
	{
		AddMessageLog(StringLog("Failure in render engine. Exiting", StringLog::Priority::Critical));
	}

	AddMessageLog(StringLog("Game now exiting", StringLog::Priority::Low));
	PrintLogs();
	glfwTerminate();
	game.reset();
	PrintLogs();

	// Still need to handle game conditions
	return engineRenderResult;
}

void BarnabusGameEngine::SetKeyCallback(std::function<void(int, int)> callback)
{
	keyCallback = callback;
	glfwSetKeyCallback(GetWindow(), key_callback);
}

std::function<void(int, int)> BarnabusGameEngine::GetKeyCallback()
{
	return keyCallback;
}