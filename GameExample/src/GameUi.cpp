#include "GameUi.h"
#include "UiShader.h"
#include "FontShader.h"
#include "VkFontShader.h"
#include "VkUIShader.h"
#include "ShaderFactory.h"
#include <GameEngine/TextQuad.h>
#include <GameEngine/BarnabusGameEngine.h>
#include <GameEngine/OpenGLTexture.h>
GameUi::GameUi()
{
	// Create initial quad.
	UiQuad* depthTextureInformation = new UiQuad(glm::vec2(0.6,0.6),glm::vec2(1,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("depthTexture", depthTextureInformation));
	//TextQuad* example = new TextQuad(glm::vec2(-1, -1), glm::vec2(1,1));
	//uiElements.insert(std::pair<std::string, UiQuad*>("example", example));
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
		ShaderFactory::CreateShader<VkUIShader>("ui", "res\\Shaders\\Vulkan\\VkUI");
	//	ShaderFactory::CreateShader<VkFontShader>("font", "res\\Shaders\\Vulkan\\VkFont");
	}

	uiElements.at("depthTexture")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("ui"));
	//uiElements.at("example")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("font"));

	ITexture* texture = nullptr;
	if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::OpenGL)
	{
		texture = new OpenGLTexture;
		texture->LoadTexture("res\\textures\\GameFont.png");
		uiElements.at("depthTexture")->GetMeshData().SetTexture(BarnabusGameEngine::Get().GetRenderer()->GetFrameBuffer("main")->GetDepthTexture());
	}
	else if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::Vulkan)
	{
		//texture = new VulkanTexture;
		//texture->LoadTexture("res\\textures\\GameFont.png");
	}
		
	//uiElements.at("depthTexture")->GetMeshData().SetTexture(texture);
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
	//uiElements.at("depthTexture")->GetMeshData().SetTexture(BarnabusGameEngine::Get().GetRenderer()->GetFrameBuffer("main")->GetDepthTexture());
	//static_cast<TextQuad*>(uiElements.at("example"))->SetText(exampleText);
	UiDisplay::Draw();
}
