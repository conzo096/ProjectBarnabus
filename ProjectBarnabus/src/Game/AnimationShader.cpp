#include "AnimationShader.h"
#include "../GameEngine/Renderer.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

void AnimationShader::UpdateUniforms(const MeshData& meshData)
{
	Use();

	// Bind Uniforms.
	const auto mvp = Renderer::Get().GetCameraVP() * glm::mat4(meshData.GetTransform());

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "MVP");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));

	for (int i = 0; i < meshData.transforms.size(); i++)
	{
		std::string uniformName("gBones[" + std::to_string(i) + "]");
		index = glGetUniformLocation(meshData.GetShader()->GetId(), uniformName.c_str());
		//std::cout << glm::to_string(meshData.transforms[i]) << std::endl;

		const auto transform = glm::mat4(meshData.transforms.at(i));
		glUniformMatrix4fv(index, 1, GL_TRUE, glm::value_ptr(transform));
	}
}
