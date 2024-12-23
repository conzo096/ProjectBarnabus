#pragma once
#include <gmock/gmock.h>
#include "GameEngine/IRenderer.h"

using namespace testing; 

class MockRenderer : public IRenderer {
public:
	MOCK_METHOD(bool, InitialiseGameEngine, (), (override));
	MOCK_METHOD(bool, ShouldWindowClose, (), (override));
	MOCK_METHOD(GLFWwindow*, GetWindow, (), (override));

	MOCK_METHOD(GraphicsRenderer, GetRenderType, (), (override));
	MOCK_METHOD(void, InitialiseMesh, (MeshData& data), (override));
	MOCK_METHOD(void, UpdateBaseVertexBuffers, (MeshData& data), (override));
	MOCK_METHOD(void, Render, (), (override));
	MOCK_METHOD(void, SetCameraViewProjection, (glm::mat4 camera), (override));
	MOCK_METHOD(void, AddMesh, (MeshData& md), (override));
	MOCK_METHOD(void, AddLight, (Light* light), (override));
	MOCK_METHOD(glm::mat4, GetCameraVP, (), (override));
	MOCK_METHOD(void, AddUiElement, (MeshData& md), (override));
	MOCK_METHOD(void, AddFramebuffer, ((std::pair<std::string, IFrameBuffer*> pair)), (override));
	MOCK_METHOD(IFrameBuffer*, GetFrameBuffer, (const std::string& buffer), (override));
	MOCK_METHOD(bool, AddShader, (std::string name, std::unique_ptr<IShader> shader), (override));
	MOCK_METHOD(IShader*, GetShader, (const std::string& shaderName), (override));
};