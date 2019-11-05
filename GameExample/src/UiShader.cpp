#include "UiShader.h"
#include <GameEngine/Renderer.h>

#include <glm\gtc\type_ptr.hpp>

void UiShader::UpdateUniforms(MeshData& meshData)
{
	Use();	
	glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());
}
