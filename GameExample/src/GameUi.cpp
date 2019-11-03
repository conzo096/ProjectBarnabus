#include "GameUi.h"
#include "UiShader.h"
#include <GameEngine/Renderer.h>
GameUi::GameUi()
{
	// Create initial quad.
	UiQuad* debugInformation = new UiQuad(glm::vec2(0.8,0.8),glm::vec2(1,1));
	auto shader = new UiShader;
	debugInformation->GetMeshData().SetShader(shader);
	uiElements.insert(std::pair<std::string, UiQuad*>("debug", debugInformation));
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
