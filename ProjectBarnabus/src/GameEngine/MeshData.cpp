#include "MeshData.h"
#include "GLShader.h"
#include "Framebuffer.h"
#include "BarnabusGameEngine.h"

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
void MeshData::SetType(GLenum meshType)
{
	type = meshType;
}

GLenum MeshData::GetType()
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


void MeshData::SetFrameBuffer(FrameBuffer * framebuffer)
{
	buffer = framebuffer;
}

FrameBuffer * MeshData::GetFrameBuffer() const
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

void MeshData::SetTexture(Texture* tex)
{
	texture = tex;
}

Texture* MeshData::GetTexture()
{
	return texture;
}
