#pragma once
#include "GameEngine/GLShader.h"

class HeightShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
};