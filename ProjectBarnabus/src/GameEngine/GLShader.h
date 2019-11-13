#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/GL.h>

class MeshData;

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
	void CreateProgram();
	void Use();
	void SetUniform(const char* name, const float val);
	void SetUniform(const char* name, const int val);

	GLuint GetUniformLocation(const char* name);
	GLuint GetUniformLocation(std::string& name);

	virtual void UpdateUniforms(MeshData& meshData);
	virtual void DrawMesh(MeshData& meshData) const;

private:
	GLint program;
	bool linked;
	std::string logString;
};