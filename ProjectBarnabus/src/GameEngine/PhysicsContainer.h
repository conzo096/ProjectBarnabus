#pragma once
#include "Entity.h"
#include "BoundingVolumes.h"
#include "Node.h"

namespace Physics
{
	class PhysicsContainer : public Component
	{
	public:
		PhysicsContainer(bool movable);
		~PhysicsContainer();
	public:
		BoundingVolumes::BoundingVolumes* GetBoundingVolume();
		void SetShader(IShader* shader);
		void InitMeshes();

		void AddBoundingVolumes(const Node* rootNode);

		void AddBoundingVolumes(const glm::vec3 minCoords, const glm::vec3 maxCoords);
		void AddBoundingVolumes(const std::vector<Vertex>& vertices, glm::mat4 trans);

	public:
		void HandleCollision(const PhysicsContainer* other, BoundingVolumes::BoundingBox& const  otherBB, BoundingVolumes::BoundingBox& const thisBB);
	public:
		void Update(float deltaTime) override;
		void Render() override;

	private:
		BoundingVolumes::BoundingVolumes boundingVolumes;
		bool isMovable;
		glm::vec3 force;
	};

}