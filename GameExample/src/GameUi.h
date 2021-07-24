#pragma once
#include <GameEngine/UiDisplay.h>

class GameUi : public UiDisplay
{
public:
	GameUi();
	~GameUi();

	void InitGameUi();
	void SetExampleText(std::string text);
	void SetEntityInfoText(std::string text);
	void UpdateBuildingPoolLimit(int currentCount, int maxCount);

	void Draw() override;
};