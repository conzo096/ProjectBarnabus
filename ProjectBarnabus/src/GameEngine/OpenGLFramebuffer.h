#pragma once
#include "IFramebuffer.h"
#include <string>

class OpenGLTexture;

class OpenGLFrameBuffer : public IFrameBuffer
{
public:
	OpenGLFrameBuffer(const std::string& frameBufferName);
	~OpenGLFrameBuffer();

	unsigned int GetFrameBuffer() override;
	ITexture* GetFrameTexture() override;
	ITexture* GetDepthTexture() override;

	void LoadFrameBuffer(int w, int h);
	void BindFrameBuffer();

private:
	std::string name;
	unsigned int bufferId;
	OpenGLTexture* frameTexture;
	OpenGLTexture* depthTexture;
	unsigned int width;
	unsigned int height;

};