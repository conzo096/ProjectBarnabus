#pragma once
#include "Singleton.h"
#include "MeshData.h"
#include "Framebuffer.h"
#include <vector>
#include <map>

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

	void AddUiElement(MeshData md);
	void AddFramebuffer(std::pair<std::string, FrameBuffer*> pair);
	FrameBuffer& GetFrameBuffer(const std::string& buffer);
	glm::vec4 GetBackgroundColour();
	void SetBackgroundColour(glm::vec4 colour);

private:
	glm::mat4 cameraVP;
	std::vector<MeshData> meshesToRender;
	std::vector<MeshData> uiElementsToRender;
	std::map<std::string, FrameBuffer*> framebuffers;
	glm::vec4 backgroundColour;

};