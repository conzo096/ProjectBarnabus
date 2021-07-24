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
	
	TextQuad* textExample = new TextQuad(glm::vec2(-1, -1), glm::vec2(1,1));
	uiElements.insert(std::pair<std::string, UiQuad*>("textExample", textExample));

	TextQuad* entityInfoText = new TextQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
	entityInfoText->SetPosition(glm::vec2(2500, 200));
	uiElements.insert(std::pair<std::string, UiQuad*>("entityInfoText", entityInfoText));


	TextQuad* buildingLimitText = new TextQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
	buildingLimitText->SetPosition(glm::vec2(500, 200));
	uiElements.insert(std::pair<std::string, UiQuad*>("BuildingPoolLimit", buildingLimitText));

	
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
		ShaderFactory::CreateShader<VkFontShader>("font", "res\\Shaders\\Vulkan\\VkFont");
	}

	uiElements.at("depthTexture")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("ui"));
	uiElements.at("textExample")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("font"));
	uiElements.at("entityInfoText")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("font"));
	uiElements.at("BuildingPoolLimit")->GetMeshData().SetShader(BarnabusGameEngine::Get().GetShader("font"));

	ITexture* texture = nullptr;
	if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::OpenGL)
	{
		texture = new OpenGLTexture;
		texture->LoadTexture("res\\textures\\GameFont.png");

		uiElements.at("depthTexture")->GetMeshData().SetTexture(BarnabusGameEngine::Get().GetRenderer()->GetFrameBuffer("main")->GetDepthTexture());
	}
	else if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::Vulkan)
	{
		texture = new VulkanTexture;
		texture->LoadTexture("res\\textures\\GameFont.png");
	}
		
	uiElements.at("textExample")->GetMeshData().SetTexture(texture);
	uiElements.at("entityInfoText")->GetMeshData().SetTexture(texture);
	uiElements.at("BuildingPoolLimit")->GetMeshData().SetTexture(texture);
}

void GameUi::SetExampleText(std::string text)
{
	static_cast<TextQuad*>(uiElements.at("textExample"))->SetText(text);
}

void GameUi::SetEntityInfoText(std::string text)
{
	static_cast<TextQuad*>(uiElements.at("entityInfoText"))->SetText(text);
}

void GameUi::UpdateBuildingPoolLimit(int currentCount, int maxCount)
{
	std::string limit = "Building cap: " + std::to_string(currentCount) + " / " + std::to_string(maxCount);

	static_cast<TextQuad*>(uiElements.at("BuildingPoolLimit"))->SetText(limit);
}

GameUi::~GameUi()
{
	for (auto& pair : uiElements)
	{
		pair.second->~UiQuad();
	}
}

