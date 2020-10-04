#include "GameUi.h"
#include "UiShader.h"
#include "FontShader.h"
#include "ShaderFactory.h"
#include <GameEngine/TextQuad.h>
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

	if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::OpenGL)
	{
		ShaderFactory::CreateShader<UiShader>("ui", "res\\shaders\\UI");
		ShaderFactory::CreateShader<FontShader>("font", "res\\shaders\\Font");
	}
	else
	{
		ShaderFactory::CreateShader<UiShader>("ui", "res\\Shaders\\Vulkan\\VkRed");
		ShaderFactory::CreateShader<FontShader>("font", "res\\Shaders\\Vulkan\\VkRed");
	}

	uiElements.at("debug")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("ui"));
	uiElements.at("example")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("font"));

	ITexture* texture = nullptr;
	if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::OpenGL)
	{
		texture = new OpenGLTexture;
		texture->LoadTexture("res\\textures\\GameFont.png");
	}
		
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
#ifdef VULKAN_RENDERER
	uiElements.at("debug")->GetMeshData().SetTexture(BarnabusGameEngine::Get().GetRenderer()->GetFrameBuffer("main")->GetDepthTexture());
	static_cast<TextQuad*>(uiElements.at("example"))->SetText(exampleText);
	UiDisplay::Draw();
#endif
}
