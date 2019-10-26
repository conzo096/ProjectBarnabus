#include <time.h>  
#include "BarnabusGame.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Terrain.h"
BarnabusGame::BarnabusGame()
{

}

BarnabusGame::~BarnabusGame()
{

}

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

	animation.SetPosition(camera.GetPosition());
	std::string fileName("res\\Models\\AnimatedModels\\LimitedWeights.dae");
	auto animatedModelComponent = std::make_unique<AnimatedModel>(fileName);
	animatedModelComponent->SetShader(animationShader);
	animatedModelComponent->InitModel();
	animatedModelComponent->SetAnimation("");
	auto cameraComponent = std::make_unique<ArcBallCamera>();
	animation.AddComponent(std::move(animatedModelComponent));

	animation.SetRotation(glm::vec3(180, animation.GetRotation().y, animation.GetRotation().z));

	camera.AddComponent(std::move(cameraComponent));

	fileName = "res\\Textures\\HeightMapExample.jpg";
	auto terrainComponent = std::make_unique<Terrain>(fileName, TerrainType::Image);
	terrainComponent->SetPosition(glm::vec3(-120, 0, -120));
	terrainComponent->SetShader(heightShader);
	terrainComponent->InitModel();
	terrain.AddComponent(std::move(terrainComponent));

	ui.InitaliseAllQuads();

	return true;
}

bool BarnabusGame::Update(double deltaTime)
{
	camera.Update(deltaTime);
	camera.GetComponent<ArcBallCamera>().SetTarget(animation.GetPosition() + glm::dvec3(0,5,0));

	animation.Update(deltaTime);
	terrain.Update(deltaTime);

	// Resolve character position - will snap to grid position
	animation.SetPosition(terrain.GetComponent<Terrain>().GetWorldPositionFromGrid(animation.GetPosition()) + glm::vec3(0, 0.5, 0));


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
	Renderer::Get().SetCameraViewProjection(camera.GetComponent<ArcBallCamera>().GetProjection() * camera.GetComponent<ArcBallCamera>().GetView());

	animation.Render();
	terrain.Render();
	Renderer::Get().Render();

	ui.Draw();
	return true;
}