#pragma once
class IGame
{
public:
	virtual ~IGame() {};
	virtual bool LoadGameContent() =0;
	virtual bool Update(float deltaTime) = 0;
	virtual bool Render(float deltaTime) = 0;
};