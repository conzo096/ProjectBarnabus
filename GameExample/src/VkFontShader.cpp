#include "VkFontShader.h"

VkFontShader::VkFontShader() : VulkanShader({ MeshData::QUAD })
{
}

void VkFontShader::Use()
{
}

void VkFontShader::UpdateUniforms(MeshData & meshData)
{
}

void VkFontShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkFontShader::GetUniformBufferSize()
{
	return VkDeviceSize();
}

VkDeviceSize VkFontShader::GetUniformItemSize()
{
	return VkDeviceSize();
}

void VkFontShader::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void VkFontShader::CreateDescriptorPool()
{
}

void VkFontShader::CreateDescriptorSets()
{
}

void VkFontShader::BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride)
{
}
