#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace VulkanUtils
{
	uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties, const VkPhysicalDevice& physicalDevice);

	VkImageView CreateImageView(const VkDevice& device, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& aspectFlags);
	void CreateImage(const VkDevice& device, const VkPhysicalDevice& physicalDevice, uint32_t width, uint32_t height, const VkFormat& format,
		const VkImageTiling& tiling, const VkImageUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkImage& image, VkDeviceMemory& imageMemory);

	void CreateBuffer(const VkDevice& device, const VkPhysicalDevice&, const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkFormat FindSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, const VkImageTiling& tiling, const VkFormatFeatureFlags& features);
	VkFormat FindDepthFormat(const VkPhysicalDevice& physicalDevice);

	template<class UniformT>
	void UpdateUniformBuffer(VkDevice& device, VkDeviceSize bufferSize, VkDeviceMemory& uniformBuffersMemory,VkBuffer& uniformBuffer, std::vector<UniformT>& uniforms )
	{
		// Make this part a templated in utils method.
		VkMemoryRequirements memoryRequirement;
		vkGetBufferMemoryRequirements(device, uniformBuffer, &memoryRequirement);

		uint8_t *data;
		vkMapMemory(device, uniformBuffersMemory, 0, sizeof(UniformT), 0, (void **)&data);

		for (int i = 0; i < uniforms.size(); i++)
		{
			memcpy(data, &uniforms[i], sizeof(uniforms[i]));
			data += bufferSize;
		}

		vkUnmapMemory(device, uniformBuffersMemory);
	}
} // namespace VulkanUtils