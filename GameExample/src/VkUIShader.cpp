#include "VkUIShader.h"

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
