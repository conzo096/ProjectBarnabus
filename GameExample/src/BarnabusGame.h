#pragma once
#include <GameEngine/IGame.h>
#include "MainEnvironment.h"

class BarnabusGame : public IGame
{
public:
	BarnabusGame();
	~BarnabusGame() {};
	bool LoadGameContent() override;
	bool Update(float deltaTime) override;
	bool Render(float deltaTime) override;

private:
	MainEnvironment mainScene;
};