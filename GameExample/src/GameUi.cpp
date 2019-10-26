#include "GameUi.h"

GameUi::GameUi()
{
	// Create initial quad.
	UiQuad* debugInformation = new UiQuad;
	uiElements.insert(std::pair<std::string, UiQuad*>("debug", debugInformation));
}

GameUi::~GameUi()
{
	for (auto& pair : uiElements)
	{
		pair.second->~UiQuad();
	}
}