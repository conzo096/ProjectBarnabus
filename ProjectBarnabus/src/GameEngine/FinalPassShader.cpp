#include "FinalPassShader.h"
#include "Renderer.h"

#include <glm\gtc\type_ptr.hpp>

void FinalPassShader::UpdateUniforms(MeshData& meshData)
{
	Use();

	GLint texLoc = glGetUniformLocation(GetId(), "gameTexture");
	glUniform1i(texLoc, 0);
	texLoc = glGetUniformLocation(GetId(), "uiTexture");
	glUniform1i(texLoc, 1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameTexture->GetTextureId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uiTexture->GetTextureId());


}
