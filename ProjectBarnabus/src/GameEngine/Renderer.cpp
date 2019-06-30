#include "Renderer.h"
#include "BarnabusGameEngine.h"

#include <glm\gtc\type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include  <glm/gtx/string_cast.hpp>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Render()
{

	glClearColor(0.1f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (int i = 0; i < meshesToRender.size(); i++)
	{
		MeshData& mesh = meshesToRender[i];
		mesh.shader.Use();

		// Bind Uniforms.
		const auto mvp = cameraVP * glm::mat4(mesh.GetTransform());
		
		std::cout << "mvp " << glm::to_string(mvp) << std::endl;
		std::cout << "View Projection " << glm::to_string(cameraVP) << std::endl;
		std::cout << "Model " << glm::to_string(mesh.GetTransform()) << std::endl;
		std::cout << "=========================================================================================================" << std::endl;

		GLint index;
		index = glGetUniformLocation(mesh.shader.GetId(), "MVP");
		glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));

		// Bind and draw model.
		glBindVertexArray(mesh.VAO);
		glDrawElements(mesh.type, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		if (i < meshesToRender.size() - 1)
		{
			if (meshesToRender[i + 1].VAO != meshesToRender[i].VAO)
			{
				glBindVertexArray(0);
			}
		}
		else
			glBindVertexArray(0);
	}

	meshesToRender.clear();
	glfwSwapBuffers(BarnabusGameEngine::Get().GetWindow());

}

void Renderer::SetFreeCamera(glm::mat4 camera)
{
	cameraVP = camera;
}
