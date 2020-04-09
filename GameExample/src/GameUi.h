#pragma once
#include <GameEngine/UiDisplay.h>

class GameUi : public UiDisplay
{
public:
	GameUi();
	~GameUi();

	void InitGameUi();
	void SetExampleText(std::string text);

	void Draw() override;

private:
	std::string exampleText;
};