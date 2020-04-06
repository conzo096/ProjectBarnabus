#include "HeightShader.h"
#include "GameEngine/Renderer.h"

#include <glm\gtc\type_ptr.hpp>

void HeightShader::UpdateUniforms(MeshData& meshData)
{
	Use();
	// Bind Uniforms.
	const auto mvp = Renderer::Get().GetCameraVP() * glm::mat4(meshData.GetTransform());

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "MVP");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));
}

void HeightShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
	UpdateUniforms(meshData);

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "lightColour");
	glUniform4fv(index, 1, glm::value_ptr(lights[0]->GetColour()));
}
