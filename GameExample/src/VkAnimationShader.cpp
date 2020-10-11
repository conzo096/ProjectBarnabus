#include "VkAnimationShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/VulkanUtils.h"

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
	
	for (int i = 0; i < 100; i++)
	{
		ubo.bones[i] = glm::mat4(1.0f);
	}
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

	attributeDescriptions[4].binding = 0;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(VertexBoneData, boneIds);

	attributeDescriptions[5].binding = 0;
	attributeDescriptions[5].location = 5;
	attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[5].offset = offsetof(VertexBoneData, weights);

	return attributeDescriptions;
}
