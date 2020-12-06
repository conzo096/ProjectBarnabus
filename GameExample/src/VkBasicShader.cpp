#include "VkBasicShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/VulkanUtils.h"
#include "GameEngine/VulkanRenderer.h"

namespace {
	const int NUMBER_OF_UNIFORMS = 50;
} //namespace

void VkBasicShader::Use()
{
	VulkanUtils::UpdateUniformBuffer<UniformBufferObject>(device, bufferSize, uniformBuffersMemory[uniformBufferIndex], uniformBuffers[uniformBufferIndex], uniforms);
	uniforms.clear();
}

void VkBasicShader::UpdateUniforms(MeshData& meshData)
{
	UniformBufferObject ubo{ BarnabusGameEngine::Get().GetRenderer()->GetCameraVP() * glm::mat4(meshData.GetTransform()) };

	uniforms.push_back(ubo);
}

void VkBasicShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkBasicShader::GetUniformBufferSize()
{
	return sizeof(UniformBufferObject) * NUMBER_OF_UNIFORMS;
}

VkDeviceSize VkBasicShader::GetUniformItemSize()
{
	return sizeof(UniformBufferObject);
}

void VkBasicShader::CreateDescriptorSetLayout()
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

void VkBasicShader::CreateDescriptorSets()
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

void VkBasicShader::CreateDescriptorPool()
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

void VkBasicShader::BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride)
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
