#pragma once
#include "IRenderer.h"

class OpenGLRenderer : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

public:
	bool InitialiseGameEngine() override;
	GLFWwindow* GetWindow() override;

private:
	GLFWwindow* window;
};