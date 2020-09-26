#pragma once
#include "Transform.h"
#include "Bone.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

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
	MeshData();
public:
	void InitialiseMesh();
	void UpdateBaseVertexBuffers();

public:

	unsigned int& GetVao();
	unsigned int& GetVbo();
	unsigned int& GetEbo();
	unsigned int& GetBones();

	void SetType(GLenum meshType);
	GLenum GetType();


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
private:

	// Skin
	GLenum type;
	unsigned int VAO, VBO, EBO, BONES;
	std::vector<unsigned int> indices;
	ITexture* texture;
	IFrameBuffer* buffer;
	// Shader for rendering the mesh.
	IShader* shader;
	// Material for lighting
	Material material;
};