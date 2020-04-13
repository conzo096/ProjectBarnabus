#include <time.h>  
#include "BarnabusGame.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Terrain.h"
#include "glm/gtx/string_cast.hpp"
BarnabusGame::BarnabusGame() : mainScene("main")
{
}
bool BarnabusGame::LoadGameContent()
{
	mainScene.LoadGameContent();
	auto cameraComponent = std::make_unique<ArcBallCamera>();
	camera.AddComponent(std::move(cameraComponent));

	ui.InitGameUi();
	ui.InitaliseAllQuads();
	return true;
}

bool BarnabusGame::Update(float deltaTime)
{
	camera.Update(deltaTime);
	camera.GetComponent<ArcBallCamera>().SetTarget(mainScene.GetEntity("player")->GetPosition() + glm::vec3(0,5,0));

	mainScene.Update(deltaTime);
	ui.SetExampleText(glm::to_string(mainScene.GetEntity("player")->GetPosition()));

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

bool BarnabusGame::Render(float deltaTime)
{
	Renderer::Get().SetCameraViewProjection(camera.GetComponent<ArcBallCamera>().GetProjection() * camera.GetComponent<ArcBallCamera>().GetView());

	mainScene.Render(deltaTime);
	ui.Draw();

	Renderer::Get().Render();

	return true;
}