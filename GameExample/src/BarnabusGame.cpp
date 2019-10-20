#include <time.h>  
#include "BarnabusGame.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"

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

	redShader.CreateProgram();
	redShader.AddShaderFromFile("res\\Shaders\\Red.vert", GLShader::VERTEX);
	redShader.AddShaderFromFile("res\\Shaders\\Red.frag", GLShader::FRAGMENT);
	redShader.Link();

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

	fileName = "res\\Models\\terrain.obj";
	auto modelComponent = std::make_unique<Model>(fileName);
	modelComponent->SetShader(redShader);
	modelComponent->InitModel();
	terrain.AddComponent(std::move(modelComponent));
	return true;
}

bool BarnabusGame::Update(double deltaTime)
{
	camera.Update(deltaTime);
	camera.GetComponent<ArcBallCamera>().SetTarget(animation.GetPosition() + glm::dvec3(0,5,0));
	animation.Update(deltaTime);
	terrain.Update(deltaTime);
	Renderer::Get().SetCameraViewProjection(camera.GetComponent<ArcBallCamera>().GetProjection() * camera.GetComponent<ArcBallCamera>().GetView());

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
	animation.Render();
	terrain.Render();
	Renderer::Get().Render();

	return true;
}