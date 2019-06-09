#pragma once

class Game
{
public:
	Game();
	~Game();
	virtual bool LoadGameContent();
	virtual bool Update(double deltaTime);
	virtual bool Render(double deltaTime);
};