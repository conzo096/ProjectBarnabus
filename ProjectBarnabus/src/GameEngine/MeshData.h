#pragma once
#include "GLShader.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

enum BUFFERS { POSITION, COLOR, NORMAL, TEX_COORD };

class MeshData : public Transform
{
public:

	void InitialiseMesh();

	GLenum type = GL_TRIANGLES;
	unsigned int VAO, VBO, EBO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> jointIds;
	std::vector<glm::vec3> vertexWeights;

	GLShader shader;

};