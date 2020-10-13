#include "VkFinalPassShader.h"
#include "VulkanUtils.h"
#include "BarnabusGameEngine.h"

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
