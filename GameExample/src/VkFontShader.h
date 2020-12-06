#pragma once
#include "GameEngine/VulkanShader.h"

class VkFontShader : public VulkanShader
{
public:
	VkFontShader();
	// Inherited via VulkanShader
	virtual void Use() override;
	virtual void UpdateUniforms(MeshData & meshData) override;
	virtual void UpdateUniforms(MeshData & meshData, const LightInfo & lights) override;
	virtual VkDeviceSize GetUniformBufferSize() override;
	virtual VkDeviceSize GetUniformItemSize() override;
};