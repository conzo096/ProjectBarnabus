#include <time.h>  
#include "BarnabusGame.h"
#include "../GameEngine/BarnabusGameEngine.h"
#include "../GameEngine/FreeCamera.h"
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

	auto cameraComponent = std::make_unique<FreeCamera>(70);
	cameraComponent->SetPosition(animation.GetPosition() - glm::dvec3(0, -5, -20));


	animation.AddComponent(std::move(modelComponent));

	animation.SetPosition(glm::vec3(0));
	animation.SetRotation(glm::vec3(180, cameraComponent->GetRotation().y, cameraComponent->GetRotation().z));
	animation.UpdateTransforms();

	camera.AddComponent(std::move(cameraComponent));

	return true;
}

bool BarnabusGame::Update(double deltaTime)
{
	camera.Update(deltaTime);
	animation.Update(deltaTime);
	Renderer::Get().SetFreeCamera(camera.GetComponent<FreeCamera>().GetProjection() * camera.GetComponent<FreeCamera>().GetView());
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