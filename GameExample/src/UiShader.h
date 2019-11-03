#pragma once
#include <GameEngine/GLShader.h>

class UiShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
};