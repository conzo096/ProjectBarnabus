#include "PhysicsContainer.h"

namespace Physics
{
Physics::PhysicsContainer::PhysicsContainer(bool movable) : Component("PhysicsContainer"), isMovable(movable)
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

void PhysicsContainer::HandleCollision(const PhysicsContainer* other, BoundingVolumes::BoundingBox& const otherBB, BoundingVolumes::BoundingBox& const thisBB)
{
	if (isMovable)
	{
		glm::vec3 previousPos = GetParent()->GetPreviousPosition();
		glm::vec3 currentPos = GetParent()->GetPosition();
		glm::vec3 diff = currentPos != previousPos ? glm::normalize(currentPos - previousPos) : glm::vec3(0, 0, 0);

		//Calculate how much of an intersection there is between two boxes
		auto min = thisBB.GetMinCoordinates();
		auto otherMin = otherBB.GetMinCoordinates();
		auto distance = glm::distance(min, otherMin);
		
		GetParent()->SetPosition(GetParent()->GetPreviousPosition());
	}
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
