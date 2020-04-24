#pragma once
#include "Entity.h"
#include "MeshData.h"
#include "Model.h"
#include <limits>
namespace BoundingVolumes
{
class BoundingBox : public Transform
{
public:
	BoundingBox(glm::vec3 minCoords, glm::vec3 maxCoords);
	BoundingBox(const std::vector<Vertex>& vertices, glm::mat4 trans);
public:
	void SetShader(GLShader& shader);
	void InitMesh();

	const glm::vec3 GetMaxCoordinates();
	const glm::vec3 GetMinCoordinates();
	const glm::mat4 GetOffset();

	const float GetWidth();
	const float GetHeight();
	const float GetLength();

public:
	void Update(glm::mat4 volumeTransform);
	void Render(std::string environmentName);
private: 
	glm::vec3 minCoordinates = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxCoordinates = glm::vec3(std::numeric_limits<float>::min());

	glm::mat4 offset = glm::mat4(1);
	MeshData data;
};
}