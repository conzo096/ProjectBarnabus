#include "BarnabusGameEngine.h"
#include <iostream>

namespace BarnabusGameEngine
{

	bool BarnabusGameEngine::InitialiseGameEngine()
	{
		std::cout << "InitialiseGameEngine" << std::endl;
		return true;
	}

	bool BarnabusGameEngine::LoadGameContent()
	{
		std::cout << "LoadGameContent" << std::endl;
		return true;
	}

	bool BarnabusGameEngine::Update(double deltaTime)
	{
		std::cout << "Update with deltaTime: " << deltaTime << std::endl;
		return true;
	}

	bool BarnabusGameEngine::Render(double deltaTime)
	{
		std::cout << "Render with deltaTime: " << deltaTime << std::endl;
		return true;
	}


	bool BarnabusGameEngine::StartGameEngine()
	{
		if (!InitialiseGameEngine())
		{
			std::cout << "Game engine failed to load!" << std::endl;
		}

		if (!LoadGameContent())
		{
			std::cout << "Game content was not able to be loaded!" << std::endl;
		}

		//todo remove this with a proper condition to exit the loop.
		//todo add proper deltatime value.

		int temporaryCount = 0;
		while (temporaryCount < 10)
		{
			Update(temporaryCount);
			Render(temporaryCount);

			temporaryCount++;
		}

		return true;
	}
}
