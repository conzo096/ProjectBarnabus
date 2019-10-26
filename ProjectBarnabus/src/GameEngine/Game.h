#pragma once
class Game
{
public:
	Game();
	~Game();
	virtual bool LoadGameContent();
	virtual bool Update(float deltaTime);
	virtual bool Render(float deltaTime);

};