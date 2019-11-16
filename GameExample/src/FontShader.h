#pragma once
#include <GameEngine/GLShader.h>

class FontShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;

};