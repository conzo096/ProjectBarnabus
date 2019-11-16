#include "TextQuad.h"

TextQuad::TextQuad(glm::vec2 bottomLeft, glm::vec2 topRight) : UiQuad(bottomLeft, topRight)
{

}

void TextQuad::Draw()
{
	unsigned int length = static_cast<unsigned int>(strlen(text.c_str()));
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i < length; i++)
	{
		char character = text[i];
		double uv_x = (character % 16) / 16.0f;
		double uv_y = (character / 16) / 16.0f;
		uv_y = (1 - uv_y - 0.0625);
		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));

		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_right);

		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_left);
	}

	glBindVertexArray(mesh.GetVao());
	glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVao());
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind texture coordinates.
	glEnableVertexAttribArray(BUFFERS::TEX_COORD);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVao());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	UiQuad::Draw();
}
