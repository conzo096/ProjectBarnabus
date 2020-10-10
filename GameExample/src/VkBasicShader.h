#pragma once
#include "GameEngine/VulkanShader.h"

class VkBasicShader : public VulkanShader
{
	void Use() override;
	void UpdateUniforms(MeshData & meshData) override;

protected:
	std::vector<UniformBufferObject> uniforms;
};