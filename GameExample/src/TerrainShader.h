#pragma once
#include "GameEngine/GLShader.h"

class TerrainShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
	void UpdateUniforms(MeshData& meshData, const LightInfo& lights) override;
};