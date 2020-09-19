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
		glm::vec3 direction = currentPos != previousPos ? glm::normalize(previousPos - currentPos) : glm::vec3(0, 0, 0);
		
		force += direction;
	}
}

void PhysicsContainer::Update(float deltaTime)
{
	if (force != glm::vec3(0))
	{
		GetParent()->SetPosition(GetParent()->GetPreviousPosition() + force);
		GetParent()->UpdateTransforms();
		force = glm::vec3(0);
	}
	
	boundingVolumes.Update(GetTransform());
}

void PhysicsContainer::Render()
{
	boundingVolumes.Render(GetParent()->GetEnvironmentName());
}

}
