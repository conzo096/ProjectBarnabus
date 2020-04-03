#include <time.h>  
#include "BarnabusGame.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Terrain.h"
#include "GameEngine/Movement.h"

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


	player.AddComponent(std::make_unique<Movement>());
	player.SetPosition(camera.GetPosition());
	std::string fileName("res\\Models\\AnimatedModels\\LimitedWeights.dae");
	auto animatedModelComponent = std::make_unique<AnimatedModel>(fileName);
	animatedModelComponent->SetShader(animationShader);
	animatedModelComponent->InitModel();
	animatedModelComponent->SetAnimation("");
	auto cameraComponent = std::make_unique<ArcBallCamera>();
	
	player.AddComponent(std::move(animatedModelComponent));
	player.SetRotation(glm::vec3(180, player.GetRotation().y, player.GetRotation().z));

	camera.AddComponent(std::move(cameraComponent));

	fileName = "res\\Textures\\HeightMap.png";
	auto terrainComponent = std::make_unique<Terrain>(fileName, TerrainType::Image);
	terrainComponent->LoadTerrainFromHeightMap(fileName);
	terrainComponent->SetPosition(glm::vec3(-120, 0, -120));
	terrain.SetScale(glm::vec3(4, 2, 4));
	terrainComponent->SetShader(heightShader);
	terrainComponent->InitModel();
	terrain.AddComponent(std::move(terrainComponent));

	ui.InitGameUi();
	ui.InitaliseAllQuads();
	return true;
}

bool BarnabusGame::Update(float deltaTime)
{
	camera.Update(deltaTime);
	camera.GetComponent<ArcBallCamera>().SetTarget(player.GetPosition() + glm::vec3(0,5,0));

	player.Update(deltaTime);
	terrain.Update(deltaTime);

	// Resolve character position - will snap to grid position
	player.SetPosition(terrain.GetComponent<Terrain>().GetWorldPositionFromGrid(player.GetPosition()) + glm::vec3(0, 0.5, 0));

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

	player.Render();
	terrain.Render();
	ui.Draw();

	Renderer::Get().Render();

	return true;
}