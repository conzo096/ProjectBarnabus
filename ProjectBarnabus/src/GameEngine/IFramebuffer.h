#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include <string>

class Texture;

class IFrameBuffer
{
public:
	~IFrameBuffer()
	{
	}

	virtual unsigned int GetFrameBuffer() = 0;
	virtual Texture* GetFrameTexture() = 0;
	virtual Texture* GetDepthTexture() = 0;

	virtual void LoadFrameBuffer(int w, int h) = 0;
	virtual void BindFrameBuffer() = 0;
};