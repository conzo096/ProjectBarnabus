#pragma once
#include "GameEngine/VulkanShader.h"

class VkBasicShader : public VulkanShader
{
	// Inherited via VulkanShader
	virtual void Use() override;
	virtual void UpdateUniforms(MeshData & meshData) override;

protected:
	std::vector<UniformBufferObject> uniforms;
};