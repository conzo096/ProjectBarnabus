#pragma once
#include "IShader.h"
#include "MeshData.h"
#include "VulkanTexture.h"
#include <vulkan/vulkan.h>
#include <set>
namespace 
{
	using LightInfo = std::vector<Light*>;
}

class VulkanShader : public IShader
{
public:

	// Info gathered from meshData
	// Can be created interally in shaders that use it
	// Need size of struct - Can be extracted into a method that is pure virtual
	// Need to update buffer. 
	// Can do this by making use of the Use function -> for vulkan Calling this could update the uniform buffers then
	// Thus - Update uniforms (Creates struct, adds to a list.) - Use - Updates buffers & clears list
	// UpdateUniformsBuffers needs an index past in. Could add a setId function & make use of that?

	struct UniformBufferObject
	{
		glm::mat4 MVP;
	};
public:
	VulkanShader();
	~VulkanShader();

	unsigned int GetId() override;

	bool AddShaderFromFile(const char* fileName, IShader::GLSLSHADERTYPE type) override;
	bool Link() override;
	bool IsLinked() override;
	void CreateProgram(const std::string shaderName) override;
	void Use() = 0;
	void SetUniform(const char* name, const float val) override;
	void SetUniform(const char* name, const int val) override;
	std::string GetName() override;
	unsigned int GetUniformLocation(const char* name) override;
	unsigned int GetUniformLocation(std::string& name) override;

	virtual void UpdateUniforms(MeshData& meshData) = 0;
	virtual void UpdateUniforms(MeshData& meshData, const LightInfo& lights) = 0;
	virtual void DrawMesh(MeshData& meshData) override;

public:
	void Use(int index);

public:
	VkPipeline GetPipeline(unsigned int index);
	VkPipelineLayout GetPipelineLayout(unsigned int index);
	VkDescriptorSet& GetDescriptorSet(unsigned int index);
	VkDeviceSize GetBufferSize();

	void CleanUp();

	void CreateGraphicPipelines();

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

protected:
	void CreateDescriptorSetLayout();
protected:
	virtual VkDeviceSize GetUniformBufferSize() = 0;
	virtual VkDeviceSize GetUniformItemSize() = 0;
	virtual std::vector< VkVertexInputBindingDescription> GetBindingDescription();
	virtual	std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
protected:
	std::string name;
	VkDevice device;

	std::set<MeshData::PrimativeType> primatives;

	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;

	std::vector<VkPipelineLayout> pipelineLayout;
	std::vector<VkPipeline> graphicsPipeline;

	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool;
	VkDeviceSize bufferSize;

	int uniformBufferIndex;
};