#include "MeshData.h"
#include <GL/glew.h>

void MeshData::InitialiseMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(BUFFERS::POSITION);
	glVertexAttribPointer(BUFFERS::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glEnableVertexAttribArray(BUFFERS::COLOR);
	glVertexAttribPointer(BUFFERS::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(NORMAL);
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	// vertex texture coords
	glEnableVertexAttribArray(TEX_COORD);
	glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);


}

void MeshData::SetType(GLenum meshType)
{
	meshType = type;
}

GLenum MeshData::GetType()
{
	return type;
}

unsigned int MeshData::GetVao()
{
	return VAO;
}

unsigned int MeshData::GetVbo()
{
	return VBO;
}

unsigned int MeshData::GetEbo()
{
	return EBO;
}

void MeshData::InsertVertex(glm::vec3 vertex)
{
	vertices.push_back(vertex);
}

void MeshData::InsertColour(glm::vec4 colour)
{
	colours.push_back(colour);
}

void MeshData::InsertTextureCoordinate(glm::vec2 textureCoord)
{
	textureCoords.push_back(textureCoord);
}

void MeshData::InsertNormal(glm::vec3 normal)
{
	normals.push_back(normal);
}

void MeshData::InsertIndex(unsigned int index)
{
	indices.push_back(index);
}

std::vector<glm::vec3> MeshData::GetVertices()
{
	return vertices;
}

std::vector<glm::vec4> MeshData::GetColours()
{
	return colours;
}

std::vector<glm::vec2> MeshData::GetTextureCoordinates()
{
	return textureCoords;
}

std::vector<glm::vec3> MeshData::GetNormals()
{
	return normals;
}

std::vector<unsigned int> MeshData::GetIndices()
{
	return indices;
}

void MeshData::SetShader(GLShader* meshShader)
{
	shader = meshShader;
}

GLShader* MeshData::GetShader() const
{
	return shader;
}
