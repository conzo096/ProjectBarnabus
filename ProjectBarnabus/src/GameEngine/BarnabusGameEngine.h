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

#include "IShader.h"
#include "ITexture.h"
#include <map>

#include <GLFW/glfw3.h>

class BarnabusGameEngine : public Singleton<BarnabusGameEngine>
{
public:
	bool StartGame(std::unique_ptr<IRenderer> renderEngine);

	GLFWwindow* GetWindow();
	void SetGame(std::unique_ptr<IGame> newGame);
	int ShouldWindowClose();
	void SetPriority(StringLog::Priority priority);
	void AddMessageLog(StringLog log);

	bool AddShader(std::string name, std::unique_ptr<IShader> shader);
	IShader* GetShader(std::string name);

	bool AddTexture(std::string name, std::unique_ptr<ITexture> texture);
	bool HasTexture(std::string name);
	ITexture* GetTexture(std::string name);

	IRenderer::GraphicsRenderer GetRenderType();
	IRenderer* GetRenderer();

	// Should be in a settings class/struct
	int width = 1920;
	int height = 1080;
public:
	// For meshes
	void InitialiseMesh(MeshData& data);
	void UpdateBaseVertexBuffers(MeshData& data);

private:
	bool InitialiseGameEngine();
	void PrintLogs();
protected:
	float time;
	float lastTime;

// This should be moved to a seperate resource manager
private:
	std::map<std::string, std::unique_ptr<ITexture>> textures;

private:
	std::unique_ptr<IRenderer> renderer = nullptr;
	bool running = true;
	std::unique_ptr<IGame> game = nullptr;
	StringLog::Priority messagePriority = StringLog::Priority::AllLogs;
	std::vector<StringLog> logs;
	int frameRate = 0;
};
