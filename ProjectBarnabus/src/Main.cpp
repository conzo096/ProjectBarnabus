#include <iostream>
#include "GameEngine/BarnabusGameEngine.h"
#include "Game/BarnabusGame.h"

int main()
{
	std::cout << "Start of project" << std::endl;
	Game* myGame = new BarnabusGame;

	BarnabusGameEngine::Get().game = myGame;
	BarnabusGameEngine::Get().StartGame();
	system("pause");
	
	return 0;
}