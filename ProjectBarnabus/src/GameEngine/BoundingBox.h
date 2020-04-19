#pragma once
#include "Entity.h"
#include "MeshData.h"
#include <limits>
namespace BoundingVolumes
{
class BoundingBox : public Component
{
public:
	BoundingBox(glm::vec3 minCoords, glm::vec3 maxCoords);
	BoundingBox(std::vector<Vertex>& vertices);
public:
	void SetShader(GLShader& shader);
	void InitMesh();

public:
	void Update(float deltaTime) override;
	void Render() override;
private: 
	glm::vec3 minCoordinates = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxCoordinates = glm::vec3(std::numeric_limits<float>::min());

	MeshData data;
};
}