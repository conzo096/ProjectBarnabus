#include "VkTerrainShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/VulkanUtils.h"

namespace {
	const int NUMBER_OF_UNIFORMS = 50;
} //namespace
void VkTerrainShader::Use()
{
	VulkanUtils::UpdateUniformBuffer<UniformBufferObject>(device, bufferSize, uniformBuffersMemory[uniformBufferIndex], uniformBuffers[uniformBufferIndex], uniforms);
	uniforms.clear();
}

void VkTerrainShader::UpdateUniforms(MeshData & meshData)
{
	UniformBufferObject ubo{ BarnabusGameEngine::Get().GetRenderer()->GetCameraVP() * glm::mat4(meshData.GetTransform()) };

	uniforms.push_back(ubo);
}

void VkTerrainShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkTerrainShader::GetUniformBufferSize()
{
	return sizeof(UniformBufferObject) * NUMBER_OF_UNIFORMS;
}

VkDeviceSize VkTerrainShader::GetUniformItemSize()
{
	return sizeof(UniformBufferObject);
}
