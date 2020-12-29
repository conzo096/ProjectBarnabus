#include <time.h>  
#include "BarnabusGame.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Terrain.h"
#include "glm/gtx/string_cast.hpp"

BarnabusGame::BarnabusGame() : mainScene("main")
{
}
bool BarnabusGame::LoadGameContent()
{
	mainScene.LoadGameContent();
	return true;
}

bool BarnabusGame::Update(float deltaTime)
{
	mainScene.Update(deltaTime);

	// Close the window if it has been asked too.
	if (BarnabusGameEngine::Get().ShouldWindowClose() || glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	else
	{
		return true;
	}

	return false;
}

bool BarnabusGame::Render(float deltaTime)
{
	mainScene.Render(deltaTime);
	BarnabusGameEngine::Get().GetRenderer()->Render();

	return true;
}