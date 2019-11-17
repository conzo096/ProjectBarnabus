#include "FontShader.h"
#include <GameEngine/Renderer.h>

#include <glm\gtc\type_ptr.hpp>
void FontShader::UpdateUniforms(MeshData & meshData)
{
	Use();

	std::string text = "TESTING";
	unsigned int length = static_cast<unsigned int>(strlen(text.c_str()));
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec2> vertices;
	for (unsigned int i = 0; i < length; i++)
	{
		glm::vec2 UpLeftVertex = glm::vec2(10 + i*60, 500 + 60);
		glm::vec2 upRightVertex = glm::vec2(10 + i* 60 + 60, 500 + 60);
		glm::vec2 rightDownVertex = glm::vec2(10 + i* 60 + 60, 500);
		glm::vec2 downLeftVertex = glm::vec2(10 + i* 60, 500);

		vertices.push_back(UpLeftVertex);
		vertices.push_back(downLeftVertex);
		vertices.push_back(upRightVertex);

		vertices.push_back(rightDownVertex);
		vertices.push_back(upRightVertex);
		vertices.push_back(downLeftVertex);

		char character = text[i];
		double xTexCoord = (character % 16) / 16.0f;
		double yTexCoord = (character / 16) / 16.0f;

		glm::vec2 upLeftTextCoord = glm::vec2(xTexCoord, yTexCoord);
		glm::vec2 upRightTexCoord = glm::vec2(xTexCoord + 1.0f / 16.0f, yTexCoord);
		glm::vec2 downRightTexCoord = glm::vec2(xTexCoord + 1.0f / 16.0f, (yTexCoord + 1.0f / 16.0f));
		glm::vec2 downLeftTexCoord = glm::vec2(xTexCoord, (yTexCoord + 1.0f / 16.0f));

		UVs.push_back(upLeftTextCoord);
		UVs.push_back(downLeftTexCoord);
		UVs.push_back(upRightTexCoord);

		UVs.push_back(downRightTexCoord);
		UVs.push_back(upRightTexCoord);
		UVs.push_back(downLeftTexCoord);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLint index;
	index = glGetUniformLocation(GetId(), "font");
	glUniform1i(index, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void FontShader::DrawMesh(MeshData & meshData) const
{
	//GLShader::DrawMesh(meshData);
}
