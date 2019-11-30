#include "GameUi.h"
#include "UiShader.h"
#include "FontShader.h"
#include <GameEngine/TextQuad.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/BarnabusGameEngine.h>
GameUi::GameUi()
{
	// Create initial quad.
	UiQuad* debugInformation = new UiQuad(glm::vec2(0.6,0.6),glm::vec2(1,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("debug", debugInformation));
	TextQuad* example = new TextQuad(glm::vec2(-1, -1), glm::vec2(1,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("example", example));
}

void GameUi::InitGameUi()
{
	auto CreateShader = [](GLShader* shader, std::string shaderName)
	{
		shader->CreateProgram();
		shader->AddShaderFromFile(std::string("res\\shaders\\"+ shaderName+".vert").c_str(), GLShader::VERTEX);
		shader->AddShaderFromFile(std::string("res\\shaders\\" + shaderName + ".frag").c_str(), GLShader::FRAGMENT);
		shader->Link();
	};

	GLShader* shader = new UiShader;
	CreateShader(shader, std::string("UI"));

	GLShader* fontShader = new FontShader;
	CreateShader(fontShader, std::string("Font"));

	uiElements.at("debug")->GetMeshData().SetShader(shader);
	uiElements.at("example")->GetMeshData().SetShader(fontShader);

	Texture* texture = new Texture;
	texture->LoadTexture("res\\textures\\GameFont.png");
	uiElements.at("example")->GetMeshData().SetTexture(texture);
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
	std::string exampleText = "Example";
	static_cast<TextQuad*>(uiElements.at("example"))->SetText(exampleText);
	UiDisplay::Draw();
}
