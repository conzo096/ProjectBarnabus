#include "MeshData.h"

#include "IShader.h"
#include "IFramebuffer.h"
#include "ITexture.h"
#include "BarnabusGameEngine.h"

#include "VulkanRenderer.h"
#include "VulkanUtils.h"

MeshData::MeshData() : VAO(0), VBO(0), EBO(0), BONES(0)
{
}

void MeshData::InitialiseMesh()
{
	BarnabusGameEngine::Get().InitialiseMesh(*this);
}

void MeshData::UpdateBaseVertexBuffers()
{
	BarnabusGameEngine::Get().UpdateBaseVertexBuffers(*this);
}
void MeshData::SetType(PrimativeType meshType)
{
	type = meshType;
}

MeshData::PrimativeType MeshData::GetType()
{
	return type;
}

unsigned int& MeshData::GetVao()
{
	return VAO;
}

unsigned int& MeshData::GetVbo()
{
	return VBO;
}

unsigned int& MeshData::GetEbo()
{
	return EBO;
}

unsigned int& MeshData::GetBones()
{
	return BONES;
}

void MeshData::InsertIndex(unsigned int index)
{
	indices.push_back(index);
}

void MeshData::InsertBoneData(VertexBoneData boneData)
{
	bonesData.push_back(boneData);
}

std::vector<unsigned int>& MeshData::GetIndices()
{
	return indices;
}

std::vector<VertexBoneData>& MeshData::GetBoneData()
{
	return bonesData;
}

void MeshData::InsertVertex(Vertex vertex)
{
	vertices.push_back(vertex);
}

void MeshData::InsertVertices(std::vector<Vertex> verts)
{
	vertices = verts;
}

void MeshData::SetShader(IShader* meshShader)
{
	shader = meshShader;
}

IShader* MeshData::GetShader() const
{
	return shader;
}

void MeshData::SetMaterial(Material mat)
{
	material = mat;
}

Material MeshData::GetMaterial() const
{
	return material;
}


void MeshData::SetFrameBuffer(IFrameBuffer * framebuffer)
{
	buffer = framebuffer;
}

IFrameBuffer * MeshData::GetFrameBuffer() const
{
	return buffer;
}

void MeshData::ResizeBoneData(unsigned int size)
{
	bonesData.resize(size);
}

void MeshData::InsertBoneDataAt(unsigned int location, int vertexLocation, float vertexWeight)
{
	bonesData[location].AddBoneData(vertexLocation, vertexWeight);
}

void MeshData::SetTexture(ITexture* tex)
{
	texture = tex;
}

ITexture* MeshData::GetTexture()
{
	return texture;
}

void MeshData::CreateVertexBuffer(VkBuffer & vertexBuffer, VkDeviceMemory & vertexBufferMemory, VkCommandPool & commandPool)
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanUtils::CreateBuffer(renderer->GetDevice(),renderer->GetPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(renderer->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(renderer->GetDevice(), stagingBufferMemory);

	VulkanUtils::CreateBuffer(renderer->GetDevice(), renderer->GetPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	CopyBuffer(commandPool, stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(renderer->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(renderer->GetDevice(), stagingBufferMemory, nullptr);
}

void MeshData::CreateIndexBuffer(VkBuffer & indexBuffer, VkDeviceMemory & indexBufferMemory, VkCommandPool & commandPool)
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanUtils::CreateBuffer(renderer->GetDevice(), renderer->GetPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(renderer->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(renderer->GetDevice(), stagingBufferMemory);

	VulkanUtils::CreateBuffer(renderer->GetDevice(), renderer->GetPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	CopyBuffer(commandPool, stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(renderer->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(renderer->GetDevice(), stagingBufferMemory, nullptr);
}

void MeshData::CreateBoneBuffer(VkBuffer & boneBuffer, VkDeviceMemory & boneBufferMemory, VkCommandPool & commandPool)
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	VkDeviceSize bufferSize = sizeof(bonesData[0]) * bonesData.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanUtils::CreateBuffer(renderer->GetDevice(), renderer->GetPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(renderer->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(renderer->GetDevice(), stagingBufferMemory);

	VulkanUtils::CreateBuffer(renderer->GetDevice(), renderer->GetPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, boneBuffer, boneBufferMemory);

	CopyBuffer(commandPool, stagingBuffer, boneBuffer, bufferSize);

	vkDestroyBuffer(renderer->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(renderer->GetDevice(), stagingBufferMemory, nullptr);
}


void MeshData::CopyBuffer(VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());
	auto graphicsQueue = renderer->GetGraphicsQueue();

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(renderer->GetDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(renderer->GetDevice(), commandPool, 1, &commandBuffer);
}
