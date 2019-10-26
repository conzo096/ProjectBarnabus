#include "UiQuad.h"
#include <GL\glew.h>
#include <GL/GL.h>

UiQuad::UiQuad(): width(1),height(1), transparency(1.0f)
{
	std::vector<glm::vec3> positions
	{
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f)
	};

	std::vector<glm::vec2> tex_coords
	{
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f)
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = positions.at(i);
		v.texCoords = tex_coords.at(i);
		vertices.push_back(v);
	}
}

UiQuad::UiQuad(glm::vec2 bottomLeft, glm::vec2 topRight) : transparency(1.0f)
{
	width = topRight.x - bottomLeft.x;
	height = topRight.y - bottomLeft.y;
	
	std::vector<glm::vec3> positions
	{
		glm::vec3(topRight.x, topRight.y, 0.0f),
		glm::vec3(bottomLeft.x, topRight.y, 0.0f),
		glm::vec3(bottomLeft.x, bottomLeft.y, 0.0f),
		glm::vec3(topRight.x, bottomLeft.y, 0.0f)
	};

	std::vector<glm::vec2> tex_coords
	{
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f)
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = positions.at(i);
		v.texCoords = tex_coords.at(i);
		vertices.push_back(v);
	}
}

UiQuad::~UiQuad()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void UiQuad::InitQuad()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

void UiQuad::Draw()
{
}
