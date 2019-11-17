#pragma once
#include <GameEngine/GLShader.h>
#include <glm/glm.hpp>
class FontShader : public GLShader
{
public:
	void UpdateUniforms(MeshData& meshData) override;
public:
	void SetText(std::string textToDisplay);
	void SetPosition(glm::vec2 pos);
	void SetSize(int newSize);

private:
	glm::vec2 position = glm::vec2(40,200);
	int size = 40;
	std::string text = "Text Not Initialised";
};