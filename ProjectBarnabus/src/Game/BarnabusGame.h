#pragma once
#include "../GameEngine/Game.h"


class BarnabusGame : public Game
{
public:
	BarnabusGame();
	~BarnabusGame();

	bool LoadGameContent() override;
	bool Update(double deltaTime) override;
	bool Render(double deltaTime) override;
};