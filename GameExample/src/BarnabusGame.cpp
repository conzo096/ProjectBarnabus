#include <time.h>  
#include "BarnabusGame.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Terrain.h"
#include "EntityFactory.h"

bool BarnabusGame::LoadGameContent()
{
	animationShader.CreateProgram();
	animationShader.AddShaderFromFile("res\\Shaders\\BasicAnimation.vert",GLShader::VERTEX);
	animationShader.AddShaderFromFile("res\\Shaders\\BasicAnimation.frag", GLShader::FRAGMENT);
	animationShader.Link();

	heightShader.CreateProgram();
	heightShader.AddShaderFromFile("res\\Shaders\\Height.vert", GLShader::VERTEX);
	heightShader.AddShaderFromFile("res\\Shaders\\Height.frag", GLShader::FRAGMENT);
	heightShader.Link();


	mainScene.AddEntity("player", EntityFactory::CreatePlayer(camera.GetPosition(), animationShader));
	mainScene.AddEntity("terrain", EntityFactory::CreateTerrain(heightShader));

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

	// Resolve character position - will snap to grid position - handle in movement component
	mainScene.GetEntity("player")->SetPosition(mainScene.GetEntity("terrain")->GetComponent<Terrain>().GetWorldPositionFromGrid(mainScene.GetEntity("player")->GetPosition()) + glm::vec3(0, 0.5, 0));

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