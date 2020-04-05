#pragma once
#include "GameEngine/GLShader.h"
#include "GameEngine/BarnabusGameEngine.h"
namespace ShaderFactory
{
	template <class shaderT>
	static void CreateShader(std::string shaderName, const std::string fileLocation)
	{
		const auto vertFileLocation = fileLocation + ".vert";
		const auto fragFileLocation = fileLocation + ".frag";
		auto shader = std::make_unique<shaderT>();

		shader->CreateProgram();
		shader->AddShaderFromFile(vertFileLocation.c_str(), GLShader::VERTEX);
		shader->AddShaderFromFile(fragFileLocation.c_str(), GLShader::FRAGMENT);
		shader->Link();

		BarnabusGameEngine::Get().AddShader(shaderName, std::move(shader));
	}
}