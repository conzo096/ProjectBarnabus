#include "VkFinalPassShader.h"
#include "VulkanUtils.h"
#include "BarnabusGameEngine.h"
#include "VulkanRenderer.h"

VkFinalPassShader::VkFinalPassShader() : VulkanShader( { MeshData::QUAD })
{
}

void VkFinalPassShader::Use()
{
	VulkanUtils::UpdateUniformBuffer<FinalPassUBO>(device, bufferSize, uniformBuffersMemory[uniformBufferIndex], uniformBuffers[uniformBufferIndex], uniforms);
	uniforms.clear();
}

void VkFinalPassShader::UpdateUniforms(MeshData & meshData)
{
	FinalPassUBO ubo{ };

	uniforms.push_back(ubo);
}

void VkFinalPassShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}


VkDeviceSize VkFinalPassShader::GetUniformBufferSize()
{
	return sizeof(FinalPassUBO);
}

VkDeviceSize VkFinalPassShader::GetUniformItemSize()
{
	return sizeof(FinalPassUBO);
}

void VkFinalPassShader::CreateDescriptorPool()
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.resize(2);

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 2;
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

	setLayoutBindings[0].binding = 0;
	setLayoutBindings[0].descriptorCount = 1;
	setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	setLayoutBindings[0].pImmutableSamplers = nullptr;
	setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	setLayoutBindings[1].binding = 1;
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
	auto colorSampler = renderer->GetColorSampler();
	auto offScreenFrameBuf = renderer->GetOffscreenFrameBuffer();
	std::vector<VkDescriptorSetLayout> layouts(renderer->GetSwapChainImages().size(), descriptorSetLayout);

	// Image descriptors for the offscreen color attachments
	VkDescriptorImageInfo texDescriptorAlbedo;
	texDescriptorAlbedo.sampler = colorSampler;
	texDescriptorAlbedo.imageView = static_cast<VulkanTexture*>(offScreenFrameBuf.GetFrameTexture())->GetImageView();
	texDescriptorAlbedo.imageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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
		writeDescriptorSets[0].dstBinding = 0;
		writeDescriptorSets[0].dstArrayElement = 0;
		writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		writeDescriptorSets[0].descriptorCount = 1;
		writeDescriptorSets[0].pBufferInfo = &bufferInfo;

		writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[1].dstSet = descriptorSets[i];
		writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSets[1].dstBinding = 1;
		writeDescriptorSets[1].pImageInfo = &texDescriptorAlbedo;
		writeDescriptorSets[1].descriptorCount = 1;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}
}
