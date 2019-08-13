#pragma once
#include "Singleton.h"
#include "MeshData.h"
#include <vector>

class Renderer : public Singleton<Renderer>
{
public:

	Renderer();
	~Renderer();

	void Render();
	void SetCameraViewProjection(glm::mat4 camera);

	void AddMesh(MeshData md);
	const std::vector<MeshData>& GetMeshesToRender();

	glm::mat4 GetCameraVP();

	glm::vec4 GetBackgroundColour();
	void SetBackgroundColour(glm::vec4 colour);

private:
	glm::mat4 cameraVP;
	std::vector<MeshData> meshesToRender;

	glm::vec4 backgroundColour;
};