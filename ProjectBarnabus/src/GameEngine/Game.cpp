#include <iostream>
#include "Game.h"
#include "BarnabusGameEngine.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::LoadGameContent()
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog( "Loading game content" , StringLog::Priority::Low ));
	return false;
}

bool Game::Update(float deltaTime)
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Updating Game" , StringLog::Priority::Low ));
	return false;
}

bool Game::Render(float deltaTime)
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Rendering Game", StringLog::Priority::Low ));
	return false;
}
