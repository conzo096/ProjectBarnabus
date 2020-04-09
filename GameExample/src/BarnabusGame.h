#pragma once
#include <GameEngine/IGame.h>
#include "MainEnvironment.h"
#include "GameUi.h"

class BarnabusGame : public IGame
{
public:
	BarnabusGame();
	~BarnabusGame() {};
	bool LoadGameContent() override;
	bool Update(float deltaTime) override;
	bool Render(float deltaTime) override;

private:
	Entity camera;
	MainEnvironment mainScene;
	GameUi ui;
};