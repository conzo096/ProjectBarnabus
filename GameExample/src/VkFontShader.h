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

	// Inherited via VulkanShader
	virtual void CreateDescriptorSetLayout() override;

	// Inherited via VulkanShader
	virtual void CreateDescriptorPool() override;
	virtual void CreateDescriptorSets() override;
	virtual void BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride) override;

	VulkanTexture* texture;
};