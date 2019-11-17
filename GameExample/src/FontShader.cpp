#include "FontShader.h"
#include <GameEngine/Renderer.h>

#include <glm\gtc\type_ptr.hpp>
void FontShader::UpdateUniforms(MeshData & meshData)
{
	auto AddVertex = [&](glm::vec3 position, glm::vec2 texCoord)
	{
		Vertex vertex;
		vertex.position = position;
		vertex.texCoords = texCoord;
		meshData.vertices.push_back(vertex);
	};

	Use();

	meshData.vertices.clear();
	for (unsigned int i = 0; i < text.length(); i++)
	{
		glm::vec3 UpLeftVertex = glm::vec3(position.x + i*size, position.y + size,0);
		glm::vec3 upRightVertex = glm::vec3(position.x + i* size + size, position.y + size, 0);
		glm::vec3 rightDownVertex = glm::vec3(position.x + i* size + size, position.y, 0);
		glm::vec3 downLeftVertex = glm::vec3(position.x + i* size, position.y, 0);

		char character = text[i];
		double xTexCoord = (character % 16) / 16.0f;
		double yTexCoord = (character / 16) / 16.0f;

		glm::vec2 upLeftTextCoord = glm::vec2(xTexCoord, yTexCoord);
		glm::vec2 upRightTexCoord = glm::vec2(xTexCoord + 1.0f / 16.0f, yTexCoord);
		glm::vec2 downRightTexCoord = glm::vec2(xTexCoord + 1.0f / 16.0f, (yTexCoord + 1.0f / 16.0f));
		glm::vec2 downLeftTexCoord = glm::vec2(xTexCoord, (yTexCoord + 1.0f / 16.0f));

		AddVertex(UpLeftVertex, upLeftTextCoord);
		AddVertex(downLeftVertex, downLeftTexCoord);
		AddVertex(rightDownVertex, downRightTexCoord);
		AddVertex(upRightVertex, upRightTexCoord);
	}

	meshData.UpdateBaseVertexBuffers();

	GLint index;
	index = glGetUniformLocation(GetId(), "font");
	glUniform1i(index, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());

}

void FontShader::SetText(std::string textToDisplay)
{
	text = textToDisplay;
}

void FontShader::SetPosition(glm::vec2 pos)
{
	position = pos;
}

void FontShader::SetSize(int newSize)
{
	size = newSize;
}
