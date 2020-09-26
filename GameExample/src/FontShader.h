#pragma once
#include <GameEngine/GLShader.h>
#include <glm/glm.hpp>

class FontShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
};