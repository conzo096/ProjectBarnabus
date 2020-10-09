#pragma once
#include <string>
#include "Light.h"
#include <vector>

class MeshData;

namespace {
	using LightInfo = std::vector<Light*>;
}
class IShader
{
public:

	virtual ~IShader() {};

	enum GLSLSHADERTYPE
	{
		VERTEX, FRAGMENT, GEOMETRY
	};

	virtual unsigned int GetId() = 0;
	virtual std::string GetName() = 0;

	virtual bool AddShaderFromFile(const char* fileName, GLSLSHADERTYPE type) =0;
	virtual bool Link() = 0;
	virtual bool IsLinked() = 0;
	virtual void CreateProgram(const std::string shaderName) =0;
	virtual void Use() =0;
	virtual void SetUniform(const char* name, const float val) =0;
	virtual void SetUniform(const char* name, const int val) =0;

	virtual unsigned int GetUniformLocation(const char* name) =0;
	virtual unsigned int GetUniformLocation(std::string& name) =0;

	virtual void UpdateUniforms(MeshData& meshData) =0;
	virtual void UpdateUniforms(MeshData& meshData, const LightInfo& lights) =0;
	virtual void DrawMesh(MeshData& meshData) =0;
};