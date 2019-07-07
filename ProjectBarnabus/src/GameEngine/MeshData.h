#pragma once
#include "Transform.h"
#include "Bone.h"
#include "GLShader.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

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

	void SetShader(GLShader* meshShader);
	GLShader* GetShader() const;

	void ResizeBoneData(unsigned int size);
	void InsertBoneDataAt(unsigned int location, int vertexLocation, float vertexWeight);

	std::vector<glm::mat4> transforms;
private:

	// Skin
	GLenum type = GL_TRIANGLES;
	unsigned int VAO, VBO, EBO, BONES;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	std::vector<VertexBoneData> bonesData;

	
	// Shader for rendering the mesh.
	GLShader* shader;

};