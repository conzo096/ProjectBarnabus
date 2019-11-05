#include "AnimationShader.h"
#include "GameEngine/Renderer.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define DEBUG_VERTICES 0

void AnimationShader::UpdateUniforms(MeshData& meshData)
{
	Use();

	// Bind Uniforms.
	const auto mvp = Renderer::Get().GetCameraVP() * glm::mat4(meshData.GetTransform());

	GLint index;
	index = glGetUniformLocation(meshData.GetShader()->GetId(), "MVP");
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(mvp));

	for (int i = 0; i < meshData.transforms.size(); i++)
	{
 		std::string uniformName("bones[" + std::to_string(i) + "]");
		index = glGetUniformLocation(meshData.GetShader()->GetId(), uniformName.c_str());

		const auto& transform = meshData.transforms.at(i);
		glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(transform));
	}

	GLint texLoc = glGetUniformLocation(GetId(), "tex");
	glUniform1i(texLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, meshData.GetTexture()->GetTextureId());

#if DEBUG_VERTICES

	for (int i = 0; i < meshData.vertices.size(); i++)
	{
		auto vertex = glm::vec4(meshData.vertices[i].position, 1);
		auto weights = meshData.bonesData[i].weights;
		auto boneIds = meshData.bonesData[i].boneIds;

		glm::vec4 vertexOut =
			(meshData.transforms[boneIds[0]] * vertex) * weights[0] +
			(meshData.transforms[boneIds[1]] * vertex) * weights[1] +
			(meshData.transforms[boneIds[2]] * vertex) * weights[2] +
			(meshData.transforms[boneIds[3]] * vertex) * weights[3];

		std::cout << "====== Vertex ============" << std::endl;
		std::cout << glm::to_string(vertex) << std::endl;
		std::cout << "====== Vertex out ============" << std::endl;
		std::cout << glm::to_string(vertexOut) << std::endl;
		std::cout << "====== Vertex out (normalized) ============" << std::endl;
		std::cout << glm::to_string(glm::normalize(vertexOut)) << std::endl;
		std::cout << "====== Vertex out (with MVP) ============" << std::endl;
		std::cout << glm::to_string( mvp* vertexOut) << std::endl;
	}
	

#endif // DEBUG_VERTICES

}
