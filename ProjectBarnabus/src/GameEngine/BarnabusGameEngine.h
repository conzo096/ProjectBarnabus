/*
	The game engine which handles the more low level code. This should handle majority of the 
	glfw and opengl calls. 

	This class contains a game membeer object. This object uses the game engine to execute the game.

*/
#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include <vector>
#include "Singleton.h"
#include "Game.h"
#include "StringLog.h"
class BarnabusGameEngine : public Singleton<BarnabusGameEngine>
{
private:
	bool InitialiseGameEngine();
public:
	bool StartGame();

	GLFWwindow* GetWindow();
	void SetGame(Game* newGame);
	int ShouldWindowClose();
	void SetPriority(StringLog::Priority priority);
	void AddMessageLog(StringLog log);
private:
	void PrintLogs();
protected:
	float time;
	float lastTime;

private:
	bool running = true;
	Game* game;
	GLFWwindow* window;

	StringLog::Priority messagePriority = StringLog::Priority::None;
	std::vector<StringLog> logs;

};