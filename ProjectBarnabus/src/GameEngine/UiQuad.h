#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "GLShader.h"
#include "MeshData.h"
class UiQuad 
{
public: 
	UiQuad();
	UiQuad(glm::vec2 bottomLeft, glm::vec2 topRight);
	~UiQuad();

	void InitQuad();

	void Draw();

protected:

	float width, height;

	float transparency;
	MeshData mesh;
	GLShader shader;
};