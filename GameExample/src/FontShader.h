#pragma once
#include <GameEngine/GLShader.h>

class FontShader : public GLShader
{
public:
	FontShader()
	{
		glGenBuffers(1, &vao);
		glGenBuffers(1, &textBuffer);
	}
	void UpdateUniforms(MeshData& meshData) override;
	void DrawMesh(MeshData& meshData) const override;

	unsigned int vao;
	unsigned int textBuffer;
};