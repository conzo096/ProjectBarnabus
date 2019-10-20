#pragma once
#include "GameEngine/Game.h"
#include "GameEngine/Entity.h"
#include "AnimationShader.h"
#include "RedShader.h"
class BarnabusGame : public Game
{
public:
	BarnabusGame();
	~BarnabusGame();

	bool LoadGameContent() override;
	bool Update(double deltaTime) override;
	bool Render(double deltaTime) override;

private:
	Entity camera;
	Entity animation;
	Entity terrain;
	AnimationShader animationShader;
	RedShader redShader;

};