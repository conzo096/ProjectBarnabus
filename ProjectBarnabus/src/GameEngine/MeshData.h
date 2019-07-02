#pragma once
#include "Transform.h"
#include "GLShader.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

enum BUFFERS { POSITION, COLOR, NORMAL, TEX_COORD };

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

	void InsertVertex(glm::vec3 vertex);
	void InsertColour(glm::vec4 colour);
	void InsertTextureCoordinate(glm::vec2 textureCoord);
	void InsertNormal(glm::vec3 normal);
	void InsertIndex(unsigned int index);

	std::vector<glm::vec3> GetVertices();
	std::vector<glm::vec4> GetColours();
	std::vector<glm::vec2> GetTextureCoordinates();
	std::vector<glm::vec3> GetNormals();
	std::vector<unsigned int> GetIndices();

	void SetShader(GLShader* meshShader);

	GLShader* GetShader() const;

private:

	// Skin
	GLenum type = GL_TRIANGLES;
	unsigned int VAO, VBO, EBO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	// Skeleton
	std::vector<unsigned int> jointIds;
	std::vector<glm::vec3> vertexWeights;
	
	// Shader for rendering the mesh.
	GLShader* shader;

};