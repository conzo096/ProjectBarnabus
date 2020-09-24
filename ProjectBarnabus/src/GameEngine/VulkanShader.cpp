#include "VulkanShader.h"

VulkanShader::~VulkanShader()
{
}

unsigned int VulkanShader::GetId()
{
	return 0;
}

bool VulkanShader::AddShaderFromFile(const char * fileName, IShader::GLSLSHADERTYPE type)
{
	return false;
}

bool VulkanShader::Link()
{
	return false;
}

bool VulkanShader::IsLinked()
{
	return false;
}

void VulkanShader::CreateProgram(const std::string shaderName)
{
}

void VulkanShader::Use()
{
}

void VulkanShader::SetUniform(const char * name, const float val)
{
}

void VulkanShader::SetUniform(const char * name, const int val)
{
}

unsigned int VulkanShader::GetUniformLocation(const char * name)
{
	return 0;
}

unsigned int VulkanShader::GetUniformLocation(std::string & name)
{
	return 0;
}

void VulkanShader::UpdateUniforms(MeshData & meshData)
{
}

void VulkanShader::UpdateUniforms(MeshData & meshData, const LightInfo & lights)
{
}

void VulkanShader::DrawMesh(MeshData & meshData) const
{
}
