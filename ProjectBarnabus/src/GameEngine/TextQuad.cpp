#include "TextQuad.h"

TextQuad::TextQuad(glm::vec2 bottomLeft, glm::vec2 topRight) : UiQuad(bottomLeft, topRight)
{
}

void TextQuad::Draw()
{
	if (textChanged)
	{
		auto AddVertex = [&](glm::vec3 position, glm::vec2 texCoord)
		{
			Vertex vertex;
			vertex.position = position;
			vertex.texCoords = texCoord;
			mesh.vertices.push_back(vertex);
		};

		mesh.vertices.clear();
		for (unsigned int i = 0; i < text.length(); i++)
		{
			glm::vec3 UpLeftVertex = glm::vec3(position.x + i * size, position.y + size, 0);
			glm::vec3 upRightVertex = glm::vec3(position.x + i * size + size, position.y + size, 0);
			glm::vec3 rightDownVertex = glm::vec3(position.x + i * size + size, position.y, 0);
			glm::vec3 downLeftVertex = glm::vec3(position.x + i * size, position.y, 0);

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

		mesh.UpdateBaseVertexBuffers();
		textChanged = false;
	}

	UiQuad::Draw();
}

void TextQuad::SetText(std::string newText)
{
	text = newText;
	textChanged = true;
}
