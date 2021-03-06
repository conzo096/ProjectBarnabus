#include "UiShader.h"
#include "GameEngine/MeshData.h"
#include <GameEngine/ITexture.h>
#include <glm\gtc\type_ptr.hpp>

void UiShader::UpdateUniforms(MeshData& meshData)
{
	Use();	

	GLint index;
	index = glGetUniformLocation(GetId(), "tex");
	glUniform1i(index, 0);
	glActiveTexture(GL_TEXTURE0);
	if(meshData.GetTexture())
	{
		glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());
	}
}
