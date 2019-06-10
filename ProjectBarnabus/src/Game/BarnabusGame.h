#pragma once
#include "../GameEngine/Game.h"
#include "../GameEngine/Entity.h"
#include "../GameEngine/GLShader.h"

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
	GLShader shader;
};