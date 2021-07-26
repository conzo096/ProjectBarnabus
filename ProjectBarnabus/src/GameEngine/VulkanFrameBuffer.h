#pragma once
#include "IFramebuffer.h"
#include "VulkanTexture.h"

class VulkanFrameBuffer : public IFrameBuffer
{
public:
	VulkanFrameBuffer();
	VulkanFrameBuffer(const std::string& frameBufferName);
	~VulkanFrameBuffer();

	unsigned int GetFrameBuffer() override;
	ITexture* GetFrameTexture() override;
	ITexture* GetDepthTexture() override;

	void LoadFrameBuffer(int w, int h);
	void BindFrameBuffer();

	unsigned int GetWidth();
	unsigned int GetHeight();
	VkFramebuffer GetVulkanFrameBuffer();
	void CreateRenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkFormat format);
	void CreateFrameBuffer(VkDevice device);
public:
	VkRenderPass GetRenderPass();
private:
	unsigned int width;
	unsigned int height;
	VkRenderPass renderPass;
	VkFramebuffer frameBuffer;
	VulkanTexture frameTexture;
	VulkanTexture depthTexture;
};