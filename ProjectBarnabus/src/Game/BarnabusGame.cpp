#include <time.h>  
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include "BarnabusGame.h"
#include "../GameEngine/BarnabusGameEngine.h"
#include "../GameEngine/FreeCamera.h"
#include "../GameEngine/Model.h"
#include "../GameEngine/Renderer.h"
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

	auto cameraComponent = std::make_unique<FreeCamera>(70);
	camera.AddComponent(std::move(cameraComponent));

	std::string fileName("res\\Models\\AnimatedModels\\model.dae");
	auto modelComponent = std::make_unique<Model>(fileName);
	modelComponent->SetShader(shaderTest);

	animation.AddComponent(std::move(modelComponent));
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