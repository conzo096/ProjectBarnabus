#pragma once
#include "Transform.h"
#include "Bone.h"
#include "Texture.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class IShader;
class FrameBuffer;

enum BUFFERS { POSITION, COLOR, NORMAL, TEX_COORD, BONES, BONE_ID, BONE_WEIGHT  };

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

	void InitialiseMesh();
	void UpdateBaseVertexBuffers();

public:

	void SetType(GLenum meshType);
	GLenum GetType();

	unsigned int GetVao();
	unsigned int GetVbo();
	unsigned int GetEbo();

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
	void SetFrameBuffer(FrameBuffer* framebuffer);
	FrameBuffer* GetFrameBuffer() const;

	void ResizeBoneData(unsigned int size);
	void InsertBoneDataAt(unsigned int location, int vertexLocation, float vertexWeight);

	void SetTexture(Texture* tex);
	Texture* GetTexture();

	// Move these to private.
	std::vector<glm::mat4> transforms;
	std::vector<Vertex> vertices;
	std::vector<VertexBoneData> bonesData;
private:

	// Skin
	GLenum type;
	unsigned int VAO, VBO, EBO, BONES;
	std::vector<unsigned int> indices;
	Texture* texture;
	FrameBuffer* buffer;
	// Shader for rendering the mesh.
	IShader* shader;
	// Material for lighting
	Material material;
};