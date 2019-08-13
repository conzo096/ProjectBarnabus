#include <time.h>  
#include "BarnabusGame.h"
#include "../GameEngine/BarnabusGameEngine.h"
#include "../GameEngine/ArcBallCamera.h"
#include "../GameEngine/AnimatedModel.h"
#include "../GameEngine/Renderer.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

BarnabusGame::BarnabusGame()
{

}

BarnabusGame::~BarnabusGame()
{

}

bool BarnabusGame::LoadGameContent()
{
	shaderTest.CreateProgram();
	shaderTest.AddShaderFromFile("res\\Shaders\\Basic.vert",GLShader::VERTEX);
	shaderTest.AddShaderFromFile("res\\Shaders\\Basic.frag", GLShader::FRAGMENT);
	shaderTest.Link();

	animation.SetPosition(camera.GetPosition());
	std::string fileName("res\\Models\\AnimatedModels\\LimitedWeights.dae");
	auto modelComponent = std::make_unique<AnimatedModel>(fileName);
	modelComponent->SetShader(shaderTest);
	modelComponent->InitModel();
	modelComponent->SetAnimation("");

	auto cameraComponent = std::make_unique<ArcBallCamera>();


	animation.AddComponent(std::move(modelComponent));

	animation.SetRotation(glm::vec3(180, cameraComponent->GetRotation().y, cameraComponent->GetRotation().z));
	animation.UpdateTransforms();

	camera.AddComponent(std::move(cameraComponent));

	return true;
}

bool BarnabusGame::Update(double deltaTime)
{
	camera.Update(deltaTime);
	camera.GetComponent<ArcBallCamera>().SetTarget(animation.GetPosition());
	animation.Update(deltaTime);
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

	Renderer::Get().Render();

	return true;
}