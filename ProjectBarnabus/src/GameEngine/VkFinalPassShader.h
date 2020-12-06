#pragma once
#include "VulkanShader.h"

class VkFinalPassShader : public VulkanShader
{
	struct FinalPassUBO
	{
	};
public:
	VkFinalPassShader();


	// Inherited via VulkanShader
	virtual void Use() override;

	virtual void UpdateUniforms(MeshData & meshData) override;

	virtual void UpdateUniforms(MeshData & meshData, const LightInfo & lights) override;

	virtual VkDeviceSize GetUniformBufferSize() override;

	virtual VkDeviceSize GetUniformItemSize() override;

	void CreateDescriptorPool() override;
	void CreateDescriptorSetLayout() override;
	void CreateDescriptorSets() override;

protected:
	VkSampler colorSampler;
};