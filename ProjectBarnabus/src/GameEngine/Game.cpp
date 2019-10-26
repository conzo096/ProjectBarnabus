#include <iostream>
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::LoadGameContent()
{
	std::cout << "LoadGameContent" << std::endl;
	return false;
}

bool Game::Update(float deltaTime)
{
	std::cout << "Update with deltaTime: " << deltaTime << std::endl;
	return false;
}

bool Game::Render(float deltaTime)
{
	std::cout << "Render with deltaTime: " << deltaTime << std::endl;
	return false;
}
