#include <iostream>
#include "BarnabusGameEngine.h"

int main()
{
	std::cout << "Start of project" << std::endl;
	BarnabusGameEngine::BarnabusGameEngine myGame;

	myGame.StartGameEngine();
	system("pause");
	
	return 0;
}