#pragma once
#include "BoundingBox.h"
#include "Node.h"

namespace BoundingVolumes
{
class BoundingVolumes
{
public:
	BoundingVolumes();
public:
	std::vector<BoundingBox>& GetBoundingBoxes();
	void SetShader(IShader* shader);
	void InitMeshes();

	void AddBoundingVolumes(const Node* rootNode);

	void AddBoundingVolumes(const glm::vec3 minCoords, const glm::vec3 maxCoords);
	void AddBoundingVolumes(const std::vector<Vertex>& vertices, glm::mat4 trans);
public:
	void Update(glm::mat4 transform);
	void Render();

private:
	std::vector<BoundingBox> boundingBoxes;
};

}