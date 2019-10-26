#pragma once
#include <GameEngine/Game.h>
#include <GameEngine/Entity.h>

#include "AnimationShader.h"
#include "HeightShader.h"
#include "GameUi.h"
class BarnabusGame : public Game
{
public:
	BarnabusGame();
	~BarnabusGame();

	bool LoadGameContent() override;
	bool Update(float deltaTime) override;
	bool Render(float deltaTime) override;

private:
	Entity camera;
	Entity animation;
	Entity terrain;
	AnimationShader animationShader;
	HeightShader heightShader;
	GameUi ui;

};