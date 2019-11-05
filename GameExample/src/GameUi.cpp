#include "GameUi.h"
#include "UiShader.h"
#include <GameEngine/Renderer.h>
GameUi::GameUi()
{
	// Create initial quad.
	UiQuad* debugInformation = new UiQuad(glm::vec2(0,0),glm::vec2(1,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("debug", debugInformation));
}

void GameUi::InitGameUi()
{
	auto shader = new UiShader;
	shader->CreateProgram();
	shader->AddShaderFromFile("..\\ProjectBarnabus\\res\\shaders\\UI.vert", GLShader::VERTEX);
	shader->AddShaderFromFile("..\\ProjectBarnabus\\res\\shaders\\UI.frag", GLShader::FRAGMENT);
	shader->Link();

	uiElements.at("debug")->GetMeshData().SetShader(shader);
}

GameUi::~GameUi()
{
	for (auto& pair : uiElements)
	{
		pair.second->~UiQuad();
	}
}

void GameUi::Draw()
{
	uiElements.at("debug")->GetMeshData().SetTexture(Renderer::Get().GetFrameBuffer("main").GetDepthTexture());
	UiDisplay::Draw();
}
