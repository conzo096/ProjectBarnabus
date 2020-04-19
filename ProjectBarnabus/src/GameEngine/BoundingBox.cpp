#include "BoundingBox.h"
#include "Renderer.h"
namespace BoundingVolumes
{
BoundingBox::BoundingBox(glm::vec3 minCoords, glm::vec3 maxCoords) : Component("BoundingBox"), minCoordinates(minCoords), maxCoordinates(maxCoords)
{
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

void BoundingBox::Render()
{
	Renderer::Get().AddMesh(GetParent()->GetEnvironmentName(), data);
}
}
