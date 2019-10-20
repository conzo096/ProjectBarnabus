#include <iostream>
#include "GameEngine/BarnabusGameEngine.h"
#include "BarnabusGame.h"
int main()
{
	std::cout << "Start of project" << std::endl;
	Game* myGame = new BarnabusGame;

	BarnabusGameEngine::Get().SetGame(myGame);
	BarnabusGameEngine::Get().StartGame();

	return 0;
}