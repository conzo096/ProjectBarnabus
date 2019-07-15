#include "MeshData.h"
#include <GL/glew.h>

void MeshData::InitialiseMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(BUFFERS::POSITION);
	glVertexAttribPointer(BUFFERS::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(BUFFERS::COLOR);
	glVertexAttribPointer(BUFFERS::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	
	// vertex normals
	glEnableVertexAttribArray(BUFFERS::NORMAL);
	glVertexAttribPointer(BUFFERS::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	// vertex texture coords
	glEnableVertexAttribArray(BUFFERS::TEX_COORD);
	glVertexAttribPointer(BUFFERS::TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	
	if (bonesData.size() > 0)
	{
		// vertex bone information
		glGenBuffers(1, &BONES);
		glBindBuffer(GL_ARRAY_BUFFER, BONES);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * bonesData.size(), &bonesData[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(BUFFERS::BONE_ID);
		glVertexAttribIPointer(BUFFERS::BONE_ID, 4, GL_INT, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, boneIds));

		glEnableVertexAttribArray(BUFFERS::BONE_WEIGHT);
		glVertexAttribPointer(BUFFERS::BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, weights));
	}

	assert(glGetError() == GL_NO_ERROR);
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

void MeshData::InsertIndex(unsigned int index)
{
	indices.push_back(index);
}

void MeshData::InsertBoneData(VertexBoneData boneData)
{
	bonesData.push_back(boneData);
}

std::vector<unsigned int> MeshData::GetIndices()
{
	return indices;
}

std::vector<VertexBoneData> MeshData::GetBoneData()
{
	return bonesData;
}

void MeshData::InsertVertex(Vertex vertex)
{
	vertices.push_back(vertex);
}

void MeshData::SetShader(GLShader* meshShader)
{
	shader = meshShader;
}

GLShader* MeshData::GetShader() const
{
	return shader;
}

void MeshData::ResizeBoneData(unsigned int size)
{
	bonesData.resize(size);
}

void MeshData::InsertBoneDataAt(unsigned int location, int vertexLocation, float vertexWeight)
{
	bonesData[location].AddBoneData(vertexLocation, vertexWeight);
}