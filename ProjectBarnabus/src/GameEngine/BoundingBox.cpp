#include "BoundingBox.h"
#include "Renderer.h"
#include <algorithm>

namespace BoundingVolumes
{
BoundingBox::BoundingBox(glm::vec3 minCoords, glm::vec3 maxCoords): minCoordinates(minCoords), maxCoordinates(maxCoords)
{
}

BoundingBox::BoundingBox(std::vector<Vertex>& vertices, glm::mat4 trans) : offset(trans)
{
	for (const Vertex& vertex : vertices)
	{
		const glm::vec3& pos = vertex.position;
		minCoordinates.x = std::min(minCoordinates.x, pos.x);
		minCoordinates.y = std::min(minCoordinates.y, pos.y);
		minCoordinates.z = std::min(minCoordinates.z, pos.z);

		maxCoordinates.x = std::max(maxCoordinates.x, pos.x);
		maxCoordinates.y = std::max(maxCoordinates.y, pos.y);
		maxCoordinates.z = std::max(maxCoordinates.z, pos.z);
	}
}

void BoundingBox::InitMesh()
{	
	glm::vec3 botLeftFront =  minCoordinates;
	glm::vec3 botRightFront = glm::vec3(maxCoordinates.x, minCoordinates.y, minCoordinates.z);
	glm::vec3 topLeftFront = glm::vec3(minCoordinates.x, maxCoordinates.y, minCoordinates.z);
	glm::vec3 topRightFront = glm::vec3(maxCoordinates.x, maxCoordinates.y, minCoordinates.z);
	glm::vec3 botLeftBack =   glm::vec3(minCoordinates.x, minCoordinates.y, maxCoordinates.z);
	glm::vec3 botRightBack =  glm::vec3(maxCoordinates.x, minCoordinates.y, maxCoordinates.z);
	glm::vec3 topLeftBack = glm::vec3(minCoordinates.x, maxCoordinates.y, maxCoordinates.z);
	glm::vec3 topRightBack = maxCoordinates;


	std::vector<unsigned int> indices
	{
		0,1,3,2,
		6,4,0,2,
		3,7,6,7,
		5,1,5,4		
	};

	std::vector<Vertex> vertices;
	Vertex vert; 
	vert.color = glm::vec4(0, 0, 1, 1);
	vert.position = botLeftFront;
	vertices.push_back(vert);
	vert.position = botRightFront;
	vertices.push_back(vert);
	vert.position = topLeftFront;
	vertices.push_back(vert);
	vert.position = topRightFront;
	vertices.push_back(vert);
	vert.position = botLeftBack;
	vertices.push_back(vert);
	vert.position = botRightBack;
	vertices.push_back(vert);
	vert.position = topLeftBack;
	vertices.push_back(vert);
	vert.position = topRightBack;
	vertices.push_back(vert);

	data.InsertVertices(vertices);

	for (auto& i : indices)
		data.InsertIndex(i);

	data.SetType(GL_LINE_STRIP);
	data.InitialiseMesh();
}

const glm::vec3 BoundingBox::GetMaxCoordinates()
{
	return data.GetTransform() * glm::vec4(maxCoordinates,1);
}

const glm::vec3 BoundingBox::GetMinCoordinates()
{
	return data.GetTransform() * glm::vec4(minCoordinates, 1);;
}

const glm::mat4 BoundingBox::GetOffset()
{
	return offset;
}

const float BoundingBox::GetWidth()
{
	auto max = GetMaxCoordinates();
	auto min = GetMinCoordinates();
	auto diff = max - min;

	return std::abs(diff.x);
}

const float BoundingBox::GetHeight()
{
	auto max = GetMaxCoordinates();
	auto min = GetMinCoordinates();
	auto diff = max - min;

	return std::abs(diff.y);
}

const float BoundingBox::GetLength()
{
	auto max = GetMaxCoordinates();
	auto min = GetMinCoordinates();
	auto diff = max - min;

	return std::abs(diff.z);
}

void BoundingBox::SetShader(GLShader & shader)
{
	data.SetShader(&shader);
}

void BoundingBox::Update(glm::mat4 volumeTransform)
{
	SetTransform(volumeTransform  * offset);
	data.SetTransform(GetTransform());
}

void BoundingBox::Render(std::string environmentName)
{
	Renderer::Get().AddMesh(environmentName, data);
}
}
