#include "VkBasicShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/VulkanUtils.h"

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
