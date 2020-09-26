#include "TerrainShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/DirectionalLight.h"
#include <glm\gtc\type_ptr.hpp>

void TerrainShader::UpdateUniforms(MeshData& meshData)
{
	Use();
	// Bind Uniforms.
	const auto mvp = BarnabusGameEngine::Get().GetRenderer()->GetCameraVP() * glm::mat4(meshData.GetTransform());

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "MVP");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "M");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(meshData.GetTransform()));
}

void TerrainShader::UpdateUniforms(MeshData & meshData, const LightInfo& lights)
{
	UpdateUniforms(meshData);

	auto worldLight = static_cast<DirectionalLight*>(lights[0]);

	float intensity = 0;
	if (worldLight->GetPosition().y >= 0)
	{
		// Get direction
		glm::vec3 direction = glm::normalize((worldLight->GetPosition() - meshData.GetPosition()));
		// intensity is based on how close it is to being directly above
		intensity = direction.y;
	}

	glm::vec4 worldLightColour = worldLight->GetColour() * intensity;
	worldLightColour.a = 1;
	GLint index;
	index = glGetUniformLocation(GetId(), "worldLight.colour");
	glUniform4fv(index, 1, glm::value_ptr(worldLightColour));
	index = glGetUniformLocation(GetId(), "worldLight.position");
	glUniform3fv(index, 1, glm::value_ptr(worldLight->GetPosition()));

	BindMaterial(meshData.GetMaterial());
}

void TerrainShader::BindMaterial(const Material& material)
{
	GLint id;
	id = glGetUniformLocation(GetId(), "material.emissive");
	glUniform4fv(id, 1, value_ptr(material.emissive));
	id = glGetUniformLocation(GetId(), "material.diffuse");
	glUniform4fv(id, 1, value_ptr(material.diffuse));
	id = glGetUniformLocation(GetId(), "material.specular");
	glUniform4fv(id, 1, value_ptr(material.specular));
	id = glGetUniformLocation(GetId(), "material.shininess");
	glUniform1f(id, material.shininess * 128);
}