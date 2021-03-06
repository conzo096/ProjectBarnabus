#pragma once
#include "Transform.h"
#include "Bone.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#include <vulkan/vulkan.h>

class IShader;
class IFrameBuffer;
class ITexture;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 color;
};


class MeshData : public Transform
{
public:
	enum PrimativeType
	{
		TRIANGLE,
		LINE_STRIP,
		QUAD
	};
public:
	MeshData();
public:
	void InitialiseMesh();
	void UpdateBaseVertexBuffers();

public:

	unsigned int& GetVao();
	unsigned int& GetVbo();
	unsigned int& GetEbo();
	unsigned int& GetBones();

	void SetType(PrimativeType meshType);
	PrimativeType GetType();


	void InsertIndex(unsigned int index);
	void InsertBoneData(VertexBoneData boneData);

	std::vector<unsigned int>& GetIndices();
	std::vector<VertexBoneData>& GetBoneData();

	void InsertVertex(Vertex vertex);
	void InsertVertices(std::vector<Vertex> verts);
	void SetShader(IShader* meshShader);
	IShader* GetShader() const;
	void SetMaterial(Material mat);
	Material GetMaterial() const;
	void SetFrameBuffer(IFrameBuffer* framebuffer);
	IFrameBuffer* GetFrameBuffer() const;

	void ResizeBoneData(unsigned int size);
	void InsertBoneDataAt(unsigned int location, int vertexLocation, float vertexWeight);

	void SetTexture(ITexture* tex);
	ITexture* GetTexture();

	// Move these to private.
	std::vector<glm::mat4> transforms;
	std::vector<Vertex> vertices;
	std::vector<VertexBoneData> bonesData;

	// VulkanSpecific - need to handle deletion

	void CreateVertexBuffer(VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, VkCommandPool& commandPool);
	void CreateIndexBuffer(VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory, VkCommandPool& commandPool);
	void CreateBoneBuffer(VkBuffer & boneBuffer, VkDeviceMemory & bonexBufferMemory, VkCommandPool & commandPool);
	void CopyBuffer(VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void UpdateVertexBuffer(VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, VkCommandPool& commandPool);

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkBuffer boneBuffer;
	VkDeviceMemory boneBufferMemory;

private:
	// Skin
	PrimativeType type;
	unsigned int VAO, VBO, EBO, BONES;
	std::vector<unsigned int> indices;
	ITexture* texture;
	IFrameBuffer* buffer;
	// Shader for rendering the mesh.
	IShader* shader;
	// Material for lighting
	Material material;

};