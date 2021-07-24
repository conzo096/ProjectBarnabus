#pragma once
#include <GameEngine/UiDisplay.h>

// Watches the main game and updates the required fields
class GameUi : public UiDisplay
{
public:
	GameUi();
	~GameUi();

	void InitGameUi();
	void SetExampleText(std::string text);
	void SetEntityInfoText(std::string text);
	void UpdateBuildingPoolLimit(int currentCount, int maxCount);
	void UpdateUnitPoolLimit(int currentCount, int maxCount);
};