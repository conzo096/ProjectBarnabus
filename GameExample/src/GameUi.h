#pragma once
#include <GameEngine/UiDisplay.h>

class GameUi : public UiDisplay
{
public:
	GameUi();
	void InitGameUi();
	~GameUi();

	void Draw() override;
};