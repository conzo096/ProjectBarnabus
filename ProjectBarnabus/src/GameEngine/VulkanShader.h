#pragma once
#include "IShader.h"
#include <vulkan/vulkan.h>

namespace 
{
	using LightInfo = std::vector<Light*>;
}

class VulkanShader : public IShader
{
public:
	struct UniformBufferObject {
		glm::vec4 color;
	};
public:
	VulkanShader();
	~VulkanShader();

	unsigned int GetId() override;

	bool AddShaderFromFile(const char* fileName, IShader::GLSLSHADERTYPE type) override;
	bool Link() override;
	bool IsLinked() override;
	void CreateProgram(const std::string shaderName) override;
	void Use() override;
	void SetUniform(const char* name, const float val) override;
	void SetUniform(const char* name, const int val) override;

	unsigned int GetUniformLocation(const char* name) override;
	unsigned int GetUniformLocation(std::string& name) override;

	virtual void UpdateUniforms(MeshData& meshData) override;
	virtual void UpdateUniforms(MeshData& meshData, const LightInfo& lights) override;
	virtual void DrawMesh(MeshData& meshData) override;

	VkPipeline GetPipeline();
	VkPipelineLayout GetPipelineLayout();
	VkDescriptorSet& GetDescriptorSet(unsigned int index);

	void CreateDescriptorPool();
	void CreateDescriptorSetLayout();
	void CreateDescriptorSets();
	void CreateUniformBuffers();

	void UpdateUniformBuffers(unsigned int index, std::vector<UniformBufferObject>& uniforms);

protected:
	std::string name;
	VkDevice device;

	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool;
};