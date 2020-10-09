#pragma once

#include "MeshData.h"
#include "Light.h"
#include "IFramebuffer.h"

#include <GLFW\glfw3.h>

class IRenderer
{
public:
	enum GraphicsRenderer
	{
		OpenGL,
		Vulkan
	};

public:
	virtual ~IRenderer()
	{
	}

	// For starting up the game engine
	virtual bool InitialiseGameEngine() = 0;
	virtual bool ShouldWindowClose() = 0;
	virtual GLFWwindow* GetWindow() = 0;

	virtual GraphicsRenderer GetRenderType() = 0;

	// For meshes
	virtual void InitialiseMesh(MeshData& data) = 0;
	virtual void UpdateBaseVertexBuffers(MeshData& data) = 0;

	// For rendering the game
	virtual void Render() = 0;
	virtual void SetCameraViewProjection(glm::mat4 camera) = 0;

	virtual void AddMesh(MeshData& md) = 0;
	virtual void AddLight(Light* light) = 0;
	virtual glm::mat4 GetCameraVP() = 0;

	virtual void AddUiElement(MeshData& md) = 0;
	virtual void AddFramebuffer(std::pair<std::string, IFrameBuffer*> pair) = 0;
	virtual IFrameBuffer* GetFrameBuffer(const std::string& buffer) = 0;

	virtual bool AddShader(std::string name, std::unique_ptr<IShader> shader) = 0;
	virtual IShader* GetShader(const std::string& shaderName) = 0;
};