#include "GameUi.h"
#include "UiShader.h"
#include "FontShader.h"
#include "ShaderFactory.h"
#include <GameEngine/TextQuad.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/BarnabusGameEngine.h>
#include <GameEngine/OpenGLTexture.h>
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
		shader->CreateProgram("ui");
		shader->AddShaderFromFile(std::string("res\\shaders\\"+ shaderName+".vert").c_str(), GLShader::VERTEX);
		shader->AddShaderFromFile(std::string("res\\shaders\\" + shaderName + ".frag").c_str(), GLShader::FRAGMENT);
		shader->Link();
	};

	ShaderFactory::CreateShader<UiShader>("ui", "res\\shaders\\UI");
	ShaderFactory::CreateShader<FontShader>("font", "res\\shaders\\Font");

	uiElements.at("debug")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("ui"));
	uiElements.at("example")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("font"));

	ITexture* texture = new OpenGLTexture;
	texture->LoadTexture("res\\textures\\GameFont.png");
	uiElements.at("example")->GetMeshData().SetTexture(texture);
}

void GameUi::SetExampleText(std::string text)
{
	exampleText = text;
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
	static_cast<TextQuad*>(uiElements.at("example"))->SetText(exampleText);
	UiDisplay::Draw();
}
