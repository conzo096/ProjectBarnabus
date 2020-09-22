#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
class IRenderer
{
public:
	enum GraphicsRenderer
	{
		OpenGL,
		Vulkan
	};

public:
	virtual ~IRenderer()
	{
	}

	virtual bool InitialiseGameEngine() = 0;
	virtual bool ShouldWindowClose() = 0;
	virtual GLFWwindow* GetWindow() = 0;

	virtual GraphicsRenderer GetRenderType() = 0;
};