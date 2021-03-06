#pragma once
#include "IRenderer.h"
#include <map>

class IShader;
class UiQuad;
class IFrameBuffer;
class Light;

class OpenGLRenderer : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

public:
	bool InitialiseGameEngine() override;
	bool ShouldWindowClose() override;
	GLFWwindow* GetWindow() override;

	GraphicsRenderer GetRenderType() override;

	// For meshes
	void InitialiseMesh(MeshData& data) override;
	void UpdateBaseVertexBuffers(MeshData& data) override;

	// For rendering the game
	void Render() override;
	void SetCameraViewProjection(glm::mat4 camera) override;

	void AddMesh(MeshData& md) override;
	void AddLight(Light* light) override;
	glm::mat4 GetCameraVP() override;

	void AddUiElement(MeshData& md) override;
	void AddFramebuffer(std::pair<std::string, IFrameBuffer*> pair) override;
	IFrameBuffer* GetFrameBuffer(const std::string& buffer) override;
private:
	GLFWwindow* window;

private:
	glm::mat4 cameraVP;
	std::map<IShader*, std::vector<MeshData>> meshesToRender;
	std::map<std::string, std::vector<Light*>> environmentLights;
	std::vector<MeshData> uiElementsToRender;
	std::map<std::string, IFrameBuffer*> framebuffers;
	glm::vec4 backgroundColour;

	UiQuad* screenQuad;
	IShader* finalShader;

	std::map<std::string, std::unique_ptr<IShader>> shaders;

	// Inherited via IRenderer
	virtual bool AddShader(std::string name, std::unique_ptr<IShader> shader) override;
	virtual IShader * GetShader(const std::string & shaderName) override;
};