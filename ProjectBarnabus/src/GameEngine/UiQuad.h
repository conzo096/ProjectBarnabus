#pragma once
#include <glm/glm.hpp>
#include <vector>

class UiQuad 
{
public: 
	UiQuad();
	UiQuad(glm::vec2 bottomLeft, glm::vec2 topRight);
	~UiQuad();

	void InitQuad();

	void Draw();

protected:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
	};

	std::vector<Vertex> vertices;

	float width, height;
	unsigned int vbo;
	unsigned int vao;
	float transparency;
	unsigned int shader, texture;
};