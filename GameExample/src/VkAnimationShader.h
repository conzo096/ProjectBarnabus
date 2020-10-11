#pragma once
#include "GameEngine/VulkanShader.h"

class VkAnimationShader : public VulkanShader
{
public:
	struct AnimationUBO
	{
		glm::mat4 MVP;
		glm::mat4 bones[100];
	};

public:
	void Use() override;
	void UpdateUniforms(MeshData & meshData) override;
	void UpdateUniforms(MeshData & meshData, const LightInfo & lights) override;
public:
	VkDeviceSize GetUniformBufferSize() override;
	VkDeviceSize GetUniformItemSize() override;
protected:
	std::vector< VkVertexInputBindingDescription> GetBindingDescription() override;
	std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() override;
private:
	std::vector<AnimationUBO> uniforms;
};