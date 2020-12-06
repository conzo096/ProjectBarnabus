#include "VkAnimationShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/VulkanUtils.h"
#include "GameEngine/VulkanRenderer.h"

namespace {
	const int NUMBER_OF_UNIFORMS = 50;
} //namespace

void VkAnimationShader::Use()
{
	VulkanUtils::UpdateUniformBuffer<AnimationUBO>(device, bufferSize, uniformBuffersMemory[uniformBufferIndex], uniformBuffers[uniformBufferIndex], uniforms);
	uniforms.clear();
}

void VkAnimationShader::UpdateUniforms(MeshData& meshData)
{
	AnimationUBO ubo{ BarnabusGameEngine::Get().GetRenderer()->GetCameraVP() * glm::mat4(meshData.GetTransform()) };
	
	for (int i = 0; i < meshData.transforms.size(); i++)
	{
		const auto& transform = meshData.transforms.at(i);
		ubo.bones[i] = transform;
	}

	uniforms.push_back(ubo);
}

void VkAnimationShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkAnimationShader::GetUniformBufferSize()
{
	return sizeof(AnimationUBO) * NUMBER_OF_UNIFORMS;
}

VkDeviceSize VkAnimationShader::GetUniformItemSize()
{
	return sizeof(AnimationUBO);
}

std::vector< VkVertexInputBindingDescription> VkAnimationShader::GetBindingDescription()
{
	std::vector<VkVertexInputBindingDescription> vertexInputBinding;
	vertexInputBinding.resize(2);

	vertexInputBinding[0].binding = 0;
	vertexInputBinding[0].stride = sizeof(Vertex);
	vertexInputBinding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vertexInputBinding[1].binding = 1;
	vertexInputBinding[1].stride = sizeof(VertexBoneData);
	vertexInputBinding[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return vertexInputBinding;
}


std::vector<VkVertexInputAttributeDescription> VkAnimationShader::GetAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
	attributeDescriptions.resize(6);
	
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, normal);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Vertex, texCoords);

	attributeDescriptions[4].binding = 1;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SINT;
	attributeDescriptions[4].offset = offsetof(VertexBoneData, boneIds);

	attributeDescriptions[5].binding = 1;
	attributeDescriptions[5].location = 5;
	attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[5].offset = offsetof(VertexBoneData, weights);

	return attributeDescriptions;
}

void VkAnimationShader::CreateDescriptorSetLayout()
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

void VkAnimationShader::CreateDescriptorSets()
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

void VkAnimationShader::CreateDescriptorPool()
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

void VkAnimationShader::BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride)
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
