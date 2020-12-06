#include "VkUIShader.h"
#include "GameEngine/VulkanRenderer.h"
#include "GameEngine/BarnabusGameEngine.h"

VkUIShader::VkUIShader() : VulkanShader({ MeshData::QUAD })
{
}

void VkUIShader::Use()
{
}

void VkUIShader::UpdateUniforms(MeshData & meshData)
{
}

void VkUIShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkUIShader::GetUniformBufferSize()
{
	return sizeof(UIUBO);
}

VkDeviceSize VkUIShader::GetUniformItemSize()
{
	return sizeof(UIUBO);
}

void VkUIShader::CreateDescriptorSetLayout()
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

void VkUIShader::CreateDescriptorSets()
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	std::vector<VkDescriptorSetLayout> layouts(renderer->GetSwapChainImages().size(), descriptorSetLayout);

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

	for (size_t i = 0; i < renderer->GetSwapChainImages().size(); i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = GetUniformItemSize();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
	}
}

void VkUIShader::CreateDescriptorPool()
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSize.descriptorCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(renderer->GetSwapChainImages().size());

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void VkUIShader::BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride)
{
	uint32_t uniformOffset[1] = { GetBufferSize() * stride };

	vkCmdBindDescriptorSets(buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		GetPipelineLayout(meshData.GetType())
		, 0,
		1,
		&GetDescriptorSet(imageIndex),
		1,
		uniformOffset);
}
