#include "UiDisplay.h"

UiDisplay::UiDisplay()
{
}

UiDisplay::~UiDisplay()
{
}


void UiDisplay::InitaliseAllQuads()
{
	for (auto& pair : uiElements)
	{
		pair.second->InitQuad();
	}
}

void UiDisplay::Draw()
{
	for (auto& pair : uiElements)
	{
		pair.second->Draw();
	}
}
