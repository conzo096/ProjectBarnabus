#pragma once
#include "IShader.h"
#include "MeshData.h"
#include "VulkanTexture.h"
#include <vulkan/vulkan.h>
#include <set>
#include <map>
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
	VulkanShader(std::set<MeshData::PrimativeType> types);
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
	void DrawMesh(MeshData& meshData, VkCommandBuffer& buffer, int imageIndex, unsigned int stride);
public:
	VkPipeline GetPipeline(MeshData::PrimativeType index);
	VkPipelineLayout GetPipelineLayout(MeshData::PrimativeType index);
	VkDescriptorSet& GetDescriptorSet(unsigned int index);
	VkDeviceSize GetBufferSize();

	void CleanUp();

	void CreateGraphicPipelines();

	void CreateUniformBuffers();
	virtual void CreateDescriptorPool() = 0;
	virtual void CreateDescriptorSets() = 0;

protected:
	virtual void CreateDescriptorSetLayout() = 0;
	virtual void BindDescriptorSet(MeshData & meshData, VkCommandBuffer& buffer, int imageIndex, unsigned int stride) = 0;
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

	std::map<MeshData::PrimativeType, VkPipelineLayout> pipelineLayout;
	std::map<MeshData::PrimativeType,VkPipeline> graphicsPipeline;

	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool;
	VkDeviceSize bufferSize;

	int uniformBufferIndex;
};