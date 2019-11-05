#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>

class Texture;

class FrameBuffer
{
public:

	FrameBuffer();
	~FrameBuffer();

	GLuint GetFrameBuffer();
	Texture* GetFrameTexture();
	Texture* GetDepthTexture();

	void LoadFrameBuffer(int w, int h);
	void BindFrameBuffer();

private:
	GLuint bufferId;
	Texture* frameTexture;
	Texture* depthTexture;
	GLuint width;
	GLuint height;

};