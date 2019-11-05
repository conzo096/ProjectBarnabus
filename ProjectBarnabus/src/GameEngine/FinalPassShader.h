#pragma once
#include "GLShader.h"

class Texture;

class FinalPassShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;

	Texture* gameTexture;
	Texture* uiTexture;
};