#include "AnimationShader.h"
#include "../GameEngine/Renderer.h"

#include <glm\gtc\type_ptr.hpp>

void AnimationShader::UpdateUniforms(const MeshData& meshData)
{
	Use();

	// Bind Uniforms.
	const auto mvp = Renderer::Get().GetCameraVP() * glm::mat4(meshData.GetTransform());

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "MVP");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));

}
