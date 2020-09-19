#include <iostream>
#include "GameEngine/OpenGLRenderer.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "BarnabusGame.h"
int main()
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Start of project", StringLog::Priority::Critical));

	std::unique_ptr<IGame> myGame = std::make_unique<BarnabusGame>();

	BarnabusGameEngine::Get().SetGame(std::move(myGame));
	BarnabusGameEngine::Get().StartGame( std::make_unique<OpenGLRenderer>() );

	return 0;
}