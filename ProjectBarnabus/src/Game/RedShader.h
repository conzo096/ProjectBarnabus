#pragma once
#include "../GameEngine/GLShader.h"

class RedShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
};