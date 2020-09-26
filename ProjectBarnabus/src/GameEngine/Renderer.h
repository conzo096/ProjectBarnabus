#pragma once
#include "Singleton.h"
#include "MeshData.h"
#include "OpenGLFrameBuffer.h"
#include "FinalPassShader.h"

#include <vector>
#include <map>
class UiQuad;

class Renderer : public Singleton<Renderer>
{
public:

	Renderer();
	~Renderer();

	void Render();
	void SetCameraViewProjection(glm::mat4 camera);

	void AddMesh(std::string environmentName, MeshData& md);
	void AddLight(std::string environmentName, Light* light);
	glm::mat4 GetCameraVP();

	void AddUiElement(MeshData& md);
	void AddFramebuffer(std::pair<std::string, OpenGLFrameBuffer*> pair);
	OpenGLFrameBuffer& GetFrameBuffer(const std::string& buffer);
	glm::vec4 GetBackgroundColour();
	void SetBackgroundColour(glm::vec4 colour);

private:
	glm::mat4 cameraVP;
	std::map<std::string,std::vector<MeshData>> meshesToRender;
	std::map<std::string, std::vector<Light*>> environmentLights;
	std::vector<MeshData> uiElementsToRender;
	std::map<std::string, OpenGLFrameBuffer*> framebuffers;
	glm::vec4 backgroundColour;

	UiQuad* screenQuad;
	FinalPassShader shader;
};