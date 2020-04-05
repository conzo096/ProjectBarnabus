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
#include "IGame.h"
#include "StringLog.h"

#include "GLShader.h"
#include <map>
class BarnabusGameEngine : public Singleton<BarnabusGameEngine>
{
private:
	bool InitialiseGameEngine();
public:
	bool StartGame();

	GLFWwindow* GetWindow();
	void SetGame(std::unique_ptr<IGame> newGame);
	int ShouldWindowClose();
	void SetPriority(StringLog::Priority priority);
	void AddMessageLog(StringLog log);

	bool AddShader(std::string name, std::unique_ptr<GLShader> shader);
	GLShader* GetShader(std::string name);

private:
	void PrintLogs();
protected:
	float time;
	float lastTime;

// This should be moved to a seperate resource manager
private:
	std::map<std::string, std::unique_ptr<GLShader>> shaders;
private:
	bool running = true;
	std::unique_ptr<IGame> game = nullptr;
	GLFWwindow* window;
	StringLog::Priority messagePriority = StringLog::Priority::None;
	std::vector<StringLog> logs;
	int frameRate = 0;
};