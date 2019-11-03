#include "UiShader.h"
#include <GameEngine/Renderer.h>

#include <glm\gtc\type_ptr.hpp>

void UiShader::UpdateUniforms(MeshData& meshData)
{
	Use();
	// Bind Uniforms.
	const auto mvp = Renderer::Get().GetCameraVP() * glm::mat4(meshData.GetTransform());

	auto mesh = meshData.GetTexture();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.GetTextureId());

}