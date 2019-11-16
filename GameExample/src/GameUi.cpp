#include "GameUi.h"
#include "UiShader.h"
#include "FontShader.h"
#include <GameEngine/TextQuad.h>
#include <GameEngine/Renderer.h>
GameUi::GameUi()
{
	// Create initial quad.
	UiQuad* debugInformation = new UiQuad(glm::vec2(0.6,0.6),glm::vec2(1,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("debug", debugInformation));
	TextQuad* frameRate = new TextQuad(glm::vec2(-1, 0.6), glm::vec2(-0.6,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("frameRate", frameRate));
}

void GameUi::InitGameUi()
{
	auto CreateShader = [](GLShader* shader, std::string shaderName)
	{
		shader->CreateProgram();
		shader->AddShaderFromFile(std::string("..\\ProjectBarnabus\\res\\shaders\\"+ shaderName+".vert").c_str(), GLShader::VERTEX);
		shader->AddShaderFromFile(std::string("..\\ProjectBarnabus\\res\\shaders\\" + shaderName + ".frag").c_str(), GLShader::FRAGMENT);
		shader->Link();
	};

	GLShader* shader = new UiShader;
	CreateShader(shader, std::string("UI"));

	GLShader* fontShader = new UiShader;
	CreateShader(fontShader, std::string("Font"));

	uiElements.at("debug")->GetMeshData().SetShader(shader);
	uiElements.at("frameRate")->GetMeshData().SetShader(fontShader);

	Texture* texture = new Texture;
	texture->LoadTexture("res\\textures\\Holstein.png");
	uiElements.at("frameRate")->GetMeshData().SetTexture(texture);
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
