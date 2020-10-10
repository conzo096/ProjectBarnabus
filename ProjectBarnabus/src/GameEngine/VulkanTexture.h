#pragma once
#include "ITexture.h"
#include <vulkan/vulkan.h>

class VulkanTexture : public ITexture
{
public:
	VulkanTexture();
	VulkanTexture(unsigned int w, unsigned int h);
	bool LoadTexture(const std::string& file) override;

protected:
	std::string filePath;
	int channels;

	//Depth buffer - Add to framebuffer class?
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
};