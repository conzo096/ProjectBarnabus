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
	void SetFreeCamera(glm::mat4 camera);

	glm::mat4 cameraVP;
	std::vector<MeshData> meshesToRender;

};