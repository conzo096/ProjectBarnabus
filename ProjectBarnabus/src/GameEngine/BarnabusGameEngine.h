/*
	The game engine which handles the more low level code. This should handle majority of the 
	glfw and opengl calls. 

	This class contains a game membeer object. This object uses the game engine to execute the game.

*/
#pragma once

#define GLEW_STATIC

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include "Singleton.h"
#include "Game.h"

class BarnabusGameEngine : public Singleton<BarnabusGameEngine>
{
private:
	bool InitialiseGameEngine();
protected:
public:
	bool StartGame();
	Game* game;
	GLFWwindow* window;
private:
	bool applicationActive;
protected:
	double time;
	double lastTime;
};