#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/GL.h>
#include "Light.h"
#include <vector>
class MeshData;

namespace {
	using LightInfo = std::vector<Light*>;
}
class GLShader
{
public:

	~GLShader();

	enum GLSLSHADERTYPE
	{
		VERTEX, FRAGMENT, GEOMETRY
	};

	GLuint GetId();

	bool AddShaderFromFile(const char* fileName, GLSLSHADERTYPE type);
	bool Link();
	bool IsLinked();
	void CreateProgram(const std::string shaderName);
	void Use();
	void SetUniform(const char* name, const float val);
	void SetUniform(const char* name, const int val);

	GLuint GetUniformLocation(const char* name);
	GLuint GetUniformLocation(std::string& name);

	virtual void UpdateUniforms(MeshData& meshData);
	virtual void UpdateUniforms(MeshData& meshData, const LightInfo& lights);
	virtual void DrawMesh(MeshData& meshData) const;

private:
	std::string name;
	GLint program;
	bool linked;
	std::string logString;
};