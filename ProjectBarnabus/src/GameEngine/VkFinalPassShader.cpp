#include "VkFinalPassShader.h"
#include "VulkanUtils.h"
#include "BarnabusGameEngine.h"
#include "VulkanRenderer.h"

VkFinalPassShader::VkFinalPassShader() : VulkanShader( { MeshData::QUAD })
{
}

void VkFinalPassShader::Use()
{
}

void VkFinalPassShader::UpdateUniforms(MeshData & meshData)
{
}

void VkFinalPassShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkFinalPassShader::GetUniformBufferSize()
{
	return 0;
}

VkDeviceSize VkFinalPassShader::GetUniformItemSize()
{
	return 0;
}

void VkFinalPassShader::CreateDescriptorPool()
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.resize(2);

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void VkFinalPassShader::CreateDescriptorSetLayout()
{
	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
	setLayoutBindings.resize(2);

	setLayoutBindings[0].binding = 1;
	setLayoutBindings[0].descriptorCount = 1;
	setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	
	setLayoutBindings[1].binding = 2;
	setLayoutBindings[1].descriptorCount = 1;
	setLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	setLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
	layoutInfo.pBindings = setLayoutBindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void VkFinalPassShader::CreateDescriptorSets()
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());
	auto offScreenFrameBuf = renderer->GetOffscreenFrameBuffer();
	auto uiFrameBuf = renderer->GetUiFrameBuffer();
	std::vector<VkDescriptorSetLayout> layouts(renderer->GetSwapChainImages().size(), descriptorSetLayout);

	// Image descriptors for the offscreen color attachments
	VkDescriptorImageInfo texDescriptorAlbedo;
	texDescriptorAlbedo.sampler = static_cast<VulkanTexture*>(offScreenFrameBuf.GetFrameTexture())->GetSampler();
	texDescriptorAlbedo.imageView = static_cast<VulkanTexture*>(offScreenFrameBuf.GetFrameTexture())->GetImageView();
	texDescriptorAlbedo.imageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Image descriptors for the ui
	VkDescriptorImageInfo texDescriptorUI;
	texDescriptorUI.sampler = static_cast<VulkanTexture*>(uiFrameBuf.GetFrameTexture())->GetSampler();
	texDescriptorUI.imageView = static_cast<VulkanTexture*>(uiFrameBuf.GetFrameTexture())->GetImageView();
	texDescriptorUI.imageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(renderer->GetSwapChainImages().size());
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	std::vector<VkWriteDescriptorSet> writeDescriptorSets;
	writeDescriptorSets.resize(2);

	for (size_t i = 0; i < renderer->GetSwapChainImages().size(); i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = GetUniformItemSize();

		writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[0].dstSet = descriptorSets[i];
		writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSets[0].dstBinding = 1;
		writeDescriptorSets[0].pImageInfo = &texDescriptorAlbedo;
		writeDescriptorSets[0].descriptorCount = 1;
		writeDescriptorSets[0].pBufferInfo = &bufferInfo;

		writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[1].dstSet = descriptorSets[i];
		writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSets[1].dstBinding = 2;
		writeDescriptorSets[1].pImageInfo = &texDescriptorUI;
		writeDescriptorSets[1].descriptorCount = 1;
		writeDescriptorSets[1].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}
}

void VkFinalPassShader::BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride)
{
	vkCmdBindDescriptorSets(buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		GetPipelineLayout(meshData.GetType())
		, 0,
		1,
		&GetDescriptorSet(imageIndex),
		0,
		0);
}
