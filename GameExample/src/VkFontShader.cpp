#include "VkFontShader.h"

VkFontShader::VkFontShader() : VulkanShader({ MeshData::QUAD })
{
}

void VkFontShader::Use()
{
}

void VkFontShader::UpdateUniforms(MeshData & meshData)
{
}

void VkFontShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

VkDeviceSize VkFontShader::GetUniformBufferSize()
{
	return VkDeviceSize();
}

VkDeviceSize VkFontShader::GetUniformItemSize()
{
	return VkDeviceSize();
}
