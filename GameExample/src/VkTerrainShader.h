#pragma once
#include "GameEngine/VulkanShader.h"

class VkTerrainShader : public VulkanShader
{
	void Use() override;
	void UpdateUniforms(MeshData & meshData) override;
	void UpdateUniforms(MeshData & meshData, const LightInfo & lights) override;

protected:
	VkDeviceSize GetUniformBufferSize() override;
	VkDeviceSize GetUniformItemSize() override;
protected:
	std::vector<UniformBufferObject> uniforms;

	// Inherited via VulkanShader
	virtual void CreateDescriptorSetLayout() override;

	// Inherited via VulkanShader
	virtual void CreateDescriptorSets() override;

	// Inherited via VulkanShader
	virtual void CreateDescriptorPool() override;

	// Inherited via VulkanShader
	virtual void BindDescriptorSet(MeshData & meshData, VkCommandBuffer & buffer, int imageIndex, unsigned int stride) override;
};