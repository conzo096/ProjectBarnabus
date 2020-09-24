#pragma once
#include "GameEngine/GLShader.h"
#include "GameEngine/Material.h"

class TerrainShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
	void UpdateUniforms(MeshData& meshData, const LightInfo& lights) override;

public:
	void BindMaterial(const Material& material);
};