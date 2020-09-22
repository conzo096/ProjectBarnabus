/*
	The game engine which handles the more low level code. This should handle majority of the 
	glfw and opengl calls. 

	This class contains a game membeer object. This object uses the game engine to execute the game.
*/
#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "Singleton.h"
#include "IGame.h"
#include "StringLog.h"

#include "IRenderer.h"

#include "GLShader.h"
#include "Texture.h"
#include <map>

class BarnabusGameEngine : public Singleton<BarnabusGameEngine>
{
public:
	bool StartGame(std::unique_ptr<IRenderer> renderEngine);

	GLFWwindow* GetWindow();
	void SetGame(std::unique_ptr<IGame> newGame);
	int ShouldWindowClose();
	void SetPriority(StringLog::Priority priority);
	void AddMessageLog(StringLog log);

	bool AddShader(std::string name, std::unique_ptr<GLShader> shader);
	GLShader* GetShader(std::string name);

	bool AddTexture(std::string name, std::unique_ptr<Texture> texture);
	bool HasTexture(std::string name);
	Texture* GetTexture(std::string name);

	IRenderer::GraphicsRenderer GetRenderType();

private:
	bool InitialiseGameEngine();
	void PrintLogs();
protected:
	float time;
	float lastTime;

// This should be moved to a seperate resource manager
private:
	std::map<std::string, std::unique_ptr<GLShader>> shaders;
	std::map<std::string, std::unique_ptr<Texture>> textures;

private:
	std::unique_ptr<IRenderer> renderer = nullptr;
	bool running = true;
	std::unique_ptr<IGame> game = nullptr;
	StringLog::Priority messagePriority = StringLog::Priority::NoLogs;
	std::vector<StringLog> logs;
	int frameRate = 0;
};