#include "Renderer.h"
#include "BarnabusGameEngine.h"
#include "GLShader.h"

Renderer::Renderer()
{
	backgroundColour = glm::vec4(0.1f, 0.0f, 0.4f, 1.0f);
	auto main = new FrameBuffer;
	main->LoadFrameBuffer(1920,1080);
	auto pair = std::pair<std::string, FrameBuffer*>(std::string("main"), main);
	AddFramebuffer(pair);
	auto ui = new FrameBuffer;
	ui->LoadFrameBuffer(1920, 1080);
	pair = std::pair<std::string, FrameBuffer*>(std::string("ui"), ui);
	AddFramebuffer(pair);
	auto finalPass = new FrameBuffer;
	finalPass->LoadFrameBuffer(1920, 1080);
	pair = std::pair<std::string, FrameBuffer*>(std::string("final"), finalPass);
	framebuffers.insert(pair);
}

Renderer::~Renderer()
{

}

void Renderer::Render()
{
	auto renderMeshes = [](auto& renderList)
	{
		for (int i = 0; i < renderList.size(); i++)
		{
			auto& mesh = renderList[i];
			mesh.GetShader()->UpdateUniforms(mesh);
			// Bind and draw model.
			mesh.GetShader()->DrawMesh(mesh);
		}
		renderList.clear();
	};

	// Render main game.
	GetFrameBuffer("main").BindFrameBuffer();

	glClearColor(backgroundColour.x,backgroundColour.y,backgroundColour.z,backgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	renderMeshes(meshesToRender);
	
	// render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	renderMeshes(uiElementsToRender);
	

	glfwSwapBuffers(BarnabusGameEngine::Get().GetWindow());

}

void Renderer::SetCameraViewProjection(glm::mat4 camera)
{
	cameraVP = camera;
}

void Renderer::AddMesh(MeshData md)
{
	meshesToRender.push_back(md);
}

const std::vector<MeshData>& Renderer::GetMeshesToRender()
{
	return meshesToRender;
}

glm::mat4 Renderer::GetCameraVP()
{
	return cameraVP;
}

void Renderer::AddUiElement(MeshData md)
{
	uiElementsToRender.push_back(md);
}

void Renderer::AddFramebuffer(std::pair<std::string, FrameBuffer*> pair)
{
	framebuffers.insert(pair);
}

FrameBuffer& Renderer::GetFrameBuffer(const std::string& buffer)
{
	return *framebuffers.at(buffer);
}

glm::vec4 Renderer::GetBackgroundColour()
{
	return backgroundColour;
}

void Renderer::SetBackgroundColour(glm::vec4 colour)
{
	backgroundColour = colour;
}
