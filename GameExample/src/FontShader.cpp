#include "FontShader.h"
#include "GameEngine/MeshData.h"
#include <GameEngine/ITexture.h>
#include <glm\gtc\type_ptr.hpp>

void FontShader::UpdateUniforms(MeshData& meshData)
{
	Use();

	GLint index;
	index = glGetUniformLocation(GetId(), "font");
	glUniform1i(index, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());
}
