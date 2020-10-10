#pragma once
#include "GameEngine/GLShader.h"

class BasicShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
};