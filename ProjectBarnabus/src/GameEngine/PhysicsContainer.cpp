#include "PhysicsContainer.h"

namespace Physics
{
Physics::PhysicsContainer::PhysicsContainer() : Component("PhysicsContainer")
{
}

PhysicsContainer::~PhysicsContainer()
{
}

BoundingVolumes::BoundingVolumes* PhysicsContainer::GetBoundingVolume()
{
	return &boundingVolumes;
}

void PhysicsContainer::SetShader(GLShader& shader)
{
	boundingVolumes.SetShader(shader);
}

void PhysicsContainer::InitMeshes()
{
	boundingVolumes.InitMeshes();
}

void PhysicsContainer::AddBoundingVolumes(const Node * rootNode)
{
	boundingVolumes.AddBoundingVolumes(rootNode);
}

void PhysicsContainer::AddBoundingVolumes(const glm::vec3 minCoords, const glm::vec3 maxCoords)
{
	boundingVolumes.AddBoundingVolumes(minCoords, maxCoords);
}

void PhysicsContainer::AddBoundingVolumes(const std::vector<Vertex>& vertices, glm::mat4 trans)
{
	boundingVolumes.AddBoundingVolumes(vertices, trans);
}

void PhysicsContainer::Update(float deltaTime)
{
	boundingVolumes.Update(GetTransform());
}

void PhysicsContainer::Render()
{
	boundingVolumes.Render(GetParent()->GetEnvironmentName());
}

}
