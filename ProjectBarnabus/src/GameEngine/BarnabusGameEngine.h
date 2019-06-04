#pragma once

#define GLEW_STATIC

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

namespace BarnabusGameEngine
{
	class BarnabusGameEngine
	{
	private:
		bool InitialiseGameEngine();
	protected:
		bool LoadGameContent();
		bool Update(double deltaTime);
		bool Render(double deltaTime);
	public:
		bool StartGameEngine();

	protected:
		GLFWwindow* window;
	};
}