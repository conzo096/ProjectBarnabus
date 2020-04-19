#pragma once
#include "Entity.h"
#include "MeshData.h"
namespace BoundingVolumes
{
class BoundingBox : public Component
{
public:
	BoundingBox(glm::vec3 minCoords, glm::vec3 maxCoords);

	void SetShader(GLShader& shader);
	void InitMesh();
public:
	void Render() override;
private: 
	glm::vec3 minCoordinates;
	glm::vec3 maxCoordinates;

	MeshData data;
};
}