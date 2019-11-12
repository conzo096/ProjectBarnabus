#include "UiDisplay.h"
#include "BarnabusGameEngine.h"
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

	BarnabusGameEngine::Get().AddMessageLog(StringLog("Initialised UI: TODO GET NAME", StringLog::Priority::Low));
}

void UiDisplay::Draw()
{
	for (auto& pair : uiElements)
	{
		pair.second->Draw();
	}
}
