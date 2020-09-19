#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
class IRenderer
{
public:
	virtual ~IRenderer()
	{
	}

	virtual bool InitialiseGameEngine() = 0;
	virtual GLFWwindow* GetWindow() = 0;
};