#pragma once
#include "IRenderer.h"

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
private:
	GLFWwindow* window;
};