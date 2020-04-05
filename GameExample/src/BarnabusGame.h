#pragma once
#include <GameEngine/IGame.h>
#include <GameEngine/Entity.h>

#include "AnimationShader.h"
#include "HeightShader.h"
#include "GameUi.h"
class BarnabusGame : public IGame
{
public:
	~BarnabusGame() {};
	bool LoadGameContent() override;
	bool Update(float deltaTime) override;
	bool Render(float deltaTime) override;

private:
	Entity camera;
	std::unique_ptr<Entity> player;
	std::unique_ptr<Entity> terrain;
	AnimationShader animationShader;
	HeightShader heightShader;
	GameUi ui;
};