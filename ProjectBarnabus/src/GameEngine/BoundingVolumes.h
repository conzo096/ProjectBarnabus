#pragma once
#include "Entity.h"
#include "BoundingBox.h"
#include "Node.h"

namespace BoundingVolumes
{
class BoundingVolumes : public Component
{
public:
	BoundingVolumes();
public:
	void SetShader(GLShader& shader);
	void InitMeshes();

	void AddBoundingVolumes(const Node* rootNode);

	void AddBoundingVolumes(const glm::vec3 minCoords, const glm::vec3 maxCoords);
	void AddBoundingVolumes(const std::vector<Vertex>& vertices, glm::mat4 trans);
public:
	void Update(float deltaTime) override;
	void Render() override;

private:
	std::vector<BoundingBox> boundingBoxes;
};

}