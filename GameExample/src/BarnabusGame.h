#pragma once
#include <GameEngine/IGame.h>
#include <GameEngine/Environment.h>

#include "AnimationShader.h"
#include "HeightShader.h"
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
	Environment mainScene;
	GameUi ui;
};