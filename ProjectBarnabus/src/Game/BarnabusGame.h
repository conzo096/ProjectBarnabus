#pragma once
#include "../GameEngine/Game.h"
#include "../GameEngine/Entity.h"

class BarnabusGame : public Game
{
private:
	Entity camera;
public:
	BarnabusGame();
	~BarnabusGame();

	bool LoadGameContent() override;
	bool Update(double deltaTime) override;
	bool Render(double deltaTime) override;
};