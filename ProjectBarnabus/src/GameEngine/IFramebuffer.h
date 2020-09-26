#pragma once
#include <string>

class ITexture;

class IFrameBuffer
{
public:
	~IFrameBuffer()
	{
	}

	virtual unsigned int GetFrameBuffer() = 0;
	virtual ITexture* GetFrameTexture() = 0;
	virtual ITexture* GetDepthTexture() = 0;

	virtual void LoadFrameBuffer(int w, int h) = 0;
	virtual void BindFrameBuffer() = 0;
};