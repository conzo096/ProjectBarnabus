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

	index = glGetUniformLocation(GetId(), "heightTexture");
	glUniform1i(index, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());

}
