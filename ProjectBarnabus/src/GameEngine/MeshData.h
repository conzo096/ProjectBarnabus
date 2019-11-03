#pragma once
#include "Transform.h"
#include "Bone.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class GLShader;
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

public:

	void SetType(GLenum meshType);
	GLenum GetType();

	unsigned int GetVao();
	unsigned int GetVbo();
	unsigned int GetEbo();

	void InsertIndex(unsigned int index);
	void InsertBoneData(VertexBoneData boneData);

	std::vector<unsigned int> GetIndices();
	std::vector<VertexBoneData> GetBoneData();

	void InsertVertex(Vertex vertex);
	void InsertVertices(std::vector<Vertex> verts);
	void SetShader(GLShader* meshShader);
	GLShader* GetShader() const;
	void SetFrameBuffer(FrameBuffer* framebuffer);
	FrameBuffer* GetFrameBuffer() const;

	void ResizeBoneData(unsigned int size);
	void InsertBoneDataAt(unsigned int location, int vertexLocation, float vertexWeight);

	void SetTexture(Texture& tex);
	const Texture& GetTexture();
	// Move these to private.
	std::vector<glm::mat4> transforms;
	std::vector<Vertex> vertices;
	std::vector<VertexBoneData> bonesData;
private:

	// Skin
	GLenum type;
	unsigned int VAO, VBO, EBO, BONES;
	std::vector<unsigned int> indices;
	Texture texture;
	FrameBuffer* buffer;
	// Shader for rendering the mesh.
	GLShader* shader;

};