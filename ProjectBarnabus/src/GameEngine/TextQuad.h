#pragma once
#include "UiQuad.h"
class TextQuad : public UiQuad
{
public:
	TextQuad(glm::vec2 bottomLeft, glm::vec2 topRight);
	void Draw() override;

public:
	void SetText(std::string newText);
	void SetPosition(glm::vec2 screenPosition);
	void SetSize(int newSize);
private:
	glm::vec2 position = glm::vec2(40, 200);
	int size = 40;
	std::string text = "Text Not Initialised";
	bool textChanged = true;
};