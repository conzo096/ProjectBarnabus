#pragma once
#include <GameEngine/UiDisplay.h>

class GameUi : public UiDisplay
{
public:
	GameUi();
	~GameUi();

	void Draw() override;
};