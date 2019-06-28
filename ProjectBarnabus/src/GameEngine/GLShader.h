#pragma once
#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <GL/GL.h>
class GLShader
{
public:

	GLShader();
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

private:
	bool FileExists(const std::string& fileName);

private:
	GLint program;
	bool linked;
	std::string logString;
};