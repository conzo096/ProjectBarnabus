#include "BoundingBox.h"
#include "Renderer.h"
#include <algorithm>

namespace BoundingVolumes
{
BoundingBox::BoundingBox(glm::vec3 minCoords, glm::vec3 maxCoords) : Component("BoundingBox"), minCoordinates(minCoords), maxCoordinates(maxCoords)
{
}

BoundingBox::BoundingBox(std::vector<Vertex>& vertices) : Component("BoundingBox")
{
	for (const Vertex& vertex : vertices)
	{
		const glm::vec3& pos = vertex.position;
		minCoordinates.x = std::min(minCoordinates.x, pos.x);
		minCoordinates.y = std::min(minCoordinates.y, pos.y);
		minCoordinates.z = std::max(minCoordinates.z, pos.z);

		maxCoordinates.x = std::max(maxCoordinates.x, pos.x);
		maxCoordinates.y = std::max(maxCoordinates.y, pos.y);
		maxCoordinates.z = std::min(maxCoordinates.z, pos.z);
	}
}

void BoundingBox::InitMesh()
{
	std::vector<unsigned int> indices
	{
		//0, 1, 2, 3, 8, // Front wall
		//4, 5, 6, 7, 8, // Back wall
		//4, 0, 6, 3, 8, // Left wall
		//1, 5, 3, 7, 8, // Right wall
		//2, 3, 6, 7, 8, // Top wall
		//0, 1, 4, 5  // Bottom wall
		7,6,3,2,0,
		6,4,7,5,
		3,1,0,5,4
	};

	std::vector<Vertex> vertices;
	Vertex vert; 
	vert.color = glm::vec4(0, 0, 1, 1);
	vert.position = minCoordinates;
	vertices.push_back(vert);
	vert.position = glm::vec3(maxCoordinates.x, minCoordinates.y, minCoordinates.z);
	vertices.push_back(vert);
	vert.position = glm::vec3(minCoordinates.x, maxCoordinates.y, minCoordinates.z);
	vertices.push_back(vert);
	vert.position = glm::vec3(maxCoordinates.x, maxCoordinates.y, minCoordinates.z);
	vertices.push_back(vert);
	vert.position = glm::vec3(minCoordinates.x, minCoordinates.y, maxCoordinates.z);
	vertices.push_back(vert);
	vert.position = glm::vec3(maxCoordinates.x, minCoordinates.y, maxCoordinates.z);
	vertices.push_back(vert);
	vert.position = glm::vec3(minCoordinates.x, maxCoordinates.y, maxCoordinates.z);
	vertices.push_back(vert);
	vert.position = maxCoordinates;
	vertices.push_back(vert);

	data.InsertVertices(vertices);

	for (auto& i : indices)
		data.InsertIndex(i);

	data.SetType(GL_LINE_STRIP);
	data.InitialiseMesh();
}

void BoundingBox::SetShader(GLShader & shader)
{
	data.SetShader(&shader);
}

void BoundingBox::Update(float deltaTime)
{
	data.SetTransform(GetTransform());
}

void BoundingBox::Render()
{
	Renderer::Get().AddMesh(GetParent()->GetEnvironmentName(), data);
}
}
