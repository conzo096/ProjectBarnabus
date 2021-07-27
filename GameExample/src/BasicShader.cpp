#include "BasicShader.h"
#include "GameEngine/BarnabusGameEngine.h"

#include <glm\gtc\type_ptr.hpp>

void BasicShader::UpdateUniforms(MeshData& meshData)
{
	Use();
	// Bind Uniforms.
	const auto mvp = BarnabusGameEngine::Get().GetRenderer()->GetCameraVP() * glm::mat4(meshData.GetTransform());

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "MVP");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));

	auto material = meshData.GetMaterial();
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "material.emissive");
	glUniform4fv(index, 1, glm::value_ptr(material.emissive));
	index = index = glGetUniformLocation(meshData.GetShader()->GetId(), "material.diffuse");
	glUniform4fv(index, 1, value_ptr(material.diffuse));
	index = index = glGetUniformLocation(meshData.GetShader()->GetId(), "material.specular");
	glUniform4fv(index, 1, value_ptr(material.specular));
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "material.shininess");
	glUniform1f(index, material.shininess * 128);
}
