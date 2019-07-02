#include "Renderer.h"
#include "BarnabusGameEngine.h"

Renderer::Renderer()
{
	backgroundColour = glm::vec4(0.1f, 0.0f, 0.4f, 1.0f);
}

Renderer::~Renderer()
{

}

void Renderer::Render()
{

	glClearColor(backgroundColour.x,backgroundColour.y,backgroundColour.z,backgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (int i = 0; i < meshesToRender.size(); i++)
	{
		MeshData& mesh = meshesToRender[i];
		mesh.GetShader()->UpdateUniforms(mesh);
		// Bind and draw model.
		mesh.GetShader()->DrawMesh(mesh);

	}

	meshesToRender.clear();
	glfwSwapBuffers(BarnabusGameEngine::Get().GetWindow());

}

void Renderer::SetFreeCamera(glm::mat4 camera)
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

glm::vec4 Renderer::GetBackgroundColour()
{
	return backgroundColour;
}

void Renderer::SetBackgroundColour(glm::vec4 colour)
{
	backgroundColour = colour;
}
