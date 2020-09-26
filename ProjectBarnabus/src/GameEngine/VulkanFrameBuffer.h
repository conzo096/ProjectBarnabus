#pragma once
#include "IFramebuffer.h"

class VulkanFrameBuffer : public IFrameBuffer
{
public:
	VulkanFrameBuffer(const std::string& frameBufferName);
	~VulkanFrameBuffer();

	unsigned int GetFrameBuffer() override;
	ITexture* GetFrameTexture() override;
	ITexture* GetDepthTexture() override;

	void LoadFrameBuffer(int w, int h);
	void BindFrameBuffer();
};