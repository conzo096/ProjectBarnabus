#pragma once
#include "IShader.h"
#include <vulkan/vulkan.h>
#include <set>
namespace 
{
	using LightInfo = std::vector<Light*>;
}

class VulkanShader : public IShader
{
public:
	enum PrimativeTypes
	{
		TRIANGLE,
		LINE_STRIP
	};

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
	void Use() override;
	void SetUniform(const char* name, const float val) override;
	void SetUniform(const char* name, const int val) override;

	unsigned int GetUniformLocation(const char* name) override;
	unsigned int GetUniformLocation(std::string& name) override;

	virtual void UpdateUniforms(MeshData& meshData) override;
	virtual void UpdateUniforms(MeshData& meshData, const LightInfo& lights) override;
	virtual void DrawMesh(MeshData& meshData) override;

	VkPipeline GetPipeline(unsigned int index);
	VkPipelineLayout GetPipelineLayout();
	VkDescriptorSet& GetDescriptorSet(unsigned int index);
	VkDeviceSize GetBufferSize();
	VkRenderPass GetRenderPass();
	VkImageView GetDepthImageView();
	
	void UpdateUniformBuffers(unsigned int index, std::vector<UniformBufferObject>& uniforms);

	void CleanUp();

	void CreateRenderPass();
	void CreateGraphicPipelines();
	void CreateDepthResources();

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

protected:
	void CreateDescriptorSetLayout();

	void CreateTextureImage();
	void CreateTextureImageView();
protected:
	std::string name;
	VkDevice device;

	std::set<PrimativeTypes> primatives;

	VkRenderPass renderPass;

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

	//Depth buffer - Add to framebuffer class?
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;

};