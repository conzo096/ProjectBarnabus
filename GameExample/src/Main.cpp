#include <iostream>
#include "GameEngine/BarnabusGameEngine.h"
#include "BarnabusGame.h"
int main()
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Start of project", StringLog::Priority::Critical));

	Game* myGame = new BarnabusGame;

	BarnabusGameEngine::Get().SetGame(myGame);
	BarnabusGameEngine::Get().StartGame();

	return 0;
}