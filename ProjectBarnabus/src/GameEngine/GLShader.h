#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include "IShader.h"

namespace {
	using LightInfo = std::vector<Light*>;
}

class GLShader : public IShader
{
public:

	~GLShader();

	unsigned int GetId() override;

	bool AddShaderFromFile(const char* fileName, IShader::GLSLSHADERTYPE type) override;
	bool Link() override;
	bool IsLinked() override;
	void CreateProgram(const std::string shaderName) override;
	void Use() override;
	void SetUniform(const char* name, const float val) override;
	void SetUniform(const char* name, const int val) override;

	unsigned int GetUniformLocation(const char* name) override;
	unsigned int GetUniformLocation(std::string& name) override;

	virtual void UpdateUniforms(MeshData& meshData) override;
	virtual void UpdateUniforms(MeshData& meshData, const LightInfo& lights) override;
	virtual void DrawMesh(MeshData& meshData) const override;

private:
	std::string name;
	GLint program;
	bool linked;
	std::string logString;
};