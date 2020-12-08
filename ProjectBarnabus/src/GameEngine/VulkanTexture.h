#pragma once
#include "ITexture.h"
#include <vulkan/vulkan.h>

class VulkanTexture : public ITexture
{
public:
	VulkanTexture();
	VulkanTexture(unsigned int w, unsigned int h);
	bool LoadTexture(const std::string& file) override;

	VkImage& GetImage();
	VkDeviceMemory& GetImageMemory();
	VkImageView& GetImageView();
	VkFormat GetFormat();
	VkSampler& GetSampler();

	void SetImageView(VkImageView imgView);
	void SetFormat(VkFormat form);
protected:
	std::string filePath;
	int channels;

	//Depth buffer - Add to framebuffer class?
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
	VkFormat format;
	VkSampler sampler;
};