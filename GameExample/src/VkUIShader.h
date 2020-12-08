#pragma once
#include "GameEngine/VulkanShader.h"

class VkUIShader : public VulkanShader
{
public:
	struct UIUBO
	{
	};
public:
	VkUIShader();

	// Inherited via VulkanShader
	virtual void Use() override;
	virtual void UpdateUniforms(MeshData & meshData) override;
	virtual void UpdateUniforms(MeshData & meshData, const LightInfo & lights) override;
	virtual VkDeviceSize GetUniformBufferSize() override;
	virtual VkDeviceSize GetUniformItemSize() override;

	// Inherited via VulkanShader
	virtual void CreateDescriptorSetLayout() override;

	// Inherited via VulkanShader
	virtual void CreateDescriptorSets() override;

	// Inherited via VulkanShader
	virtual void CreateDescriptorPool() override;

	// Inherited via VulkanShader
	virtual void BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride) override;
protected:
	// This is the texture that the shader will present. Should come from mesh data later.
	VulkanTexture* texture;
};