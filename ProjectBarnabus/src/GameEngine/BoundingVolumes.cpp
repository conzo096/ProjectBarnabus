#include "BoundingVolumes.h"

namespace BoundingVolumes
{

BoundingVolumes::BoundingVolumes::BoundingVolumes() : Component("BoundingVolumes")
{
}
	
void BoundingVolumes::SetShader(GLShader & shader)
{
	for (auto& bb : boundingBoxes)
	{
		bb.SetShader(shader);
	}
}
	
void BoundingVolumes::InitMeshes()
{
	for (auto& bb : boundingBoxes)
	{
		bb.InitMesh();
	}
}

void BoundingVolumes::AddBoundingVolumes(glm::vec3 minCoords, glm::vec3 maxCoords)
{
	boundingBoxes.push_back(BoundingBox(minCoords, maxCoords));
}

void BoundingVolumes::AddBoundingVolumes(const std::vector<Vertex>& vertices, glm::mat4 trans)
{
	boundingBoxes.push_back(BoundingBox(vertices, trans));
}

void BoundingVolumes::Update(float deltaTime)
{
	for (auto& bb : boundingBoxes)
	{
		bb.Update(deltaTime);
	}
}

void BoundingVolumes::Render()
{
	for (auto& bb : boundingBoxes)
	{
		bb.Render(GetParent()->GetEnvironmentName());
	}
}
}
