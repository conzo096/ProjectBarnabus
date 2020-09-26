#pragma once
# include "IFramebuffer.h"
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include <string>

class Texture;

class OpenGLFrameBuffer : public IFrameBuffer
{
public:
	OpenGLFrameBuffer(const std::string& frameBufferName);
	~OpenGLFrameBuffer();

	unsigned int GetFrameBuffer() override;
	Texture* GetFrameTexture() override;
	Texture* GetDepthTexture() override;

	void LoadFrameBuffer(int w, int h);
	void BindFrameBuffer();

private:
	std::string name;
	unsigned int bufferId;
	Texture* frameTexture;
	Texture* depthTexture;
	unsigned int width;
	unsigned int height;

};