#pragma once
#include "../GameEngine/GLShader.h"

class AnimationShader : public GLShader
{
public:

	void UpdateUniforms(const MeshData& meshData) override;

};