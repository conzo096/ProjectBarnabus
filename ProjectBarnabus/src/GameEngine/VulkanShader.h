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

public:
	void CreateCommandBuffers();
	void CreateVertexBuffer(VkCommandPool commandPool);
	void CreateIndexBuffer(VkCommandPool commandPool);
	std::vector<VkCommandBuffer>& GetCommandBuffers();
protected:
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

protected:
	std::string name;
	VkDevice device;

	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	std::vector<VkCommandBuffer> commandBuffers;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

};