#pragma once
#include <GameEngine/IGame.h>
#include "MainLevel.h"

class BarnabusGame : public IGame
{
public:
	BarnabusGame();
	~BarnabusGame() {};
	bool LoadGameContent() override;
	bool Update(float deltaTime) override;
	bool Render(float deltaTime) override;

private:
	MainLevel mainScene;
};