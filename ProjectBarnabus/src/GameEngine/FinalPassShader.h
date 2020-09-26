#pragma once
#include "GLShader.h"
#include "ITexture.h"

class FinalPassShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;

	ITexture* gameTexture;
	ITexture* uiTexture;
};