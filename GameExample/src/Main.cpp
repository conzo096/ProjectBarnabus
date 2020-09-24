//#define VULKAN_RENDERER
#include <iostream>
#ifdef VULKAN_RENDERER
	#include "GameEngine/VulkanRenderer.h"
#else
#include "GameEngine/OpenGLRenderer.h"
#endif

#include "GameEngine/BarnabusGameEngine.h"
#include "BarnabusGame.h"

int main()
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Start of project", StringLog::Priority::Critical));

	std::unique_ptr<IGame> myGame = std::make_unique<BarnabusGame>();

	BarnabusGameEngine::Get().SetGame(std::move(myGame));
	BarnabusGameEngine::Get().SetPriority(StringLog::Priority::AllLogs);

#ifdef VULKAN_RENDERER
	BarnabusGameEngine::Get().StartGame(std::make_unique<VulkanRenderer>());
#else
	BarnabusGameEngine::Get().StartGame(std::make_unique<OpenGLRenderer>());
#endif 
	
	return 0;
}