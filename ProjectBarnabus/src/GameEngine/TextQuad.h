#pragma once
#include "UiQuad.h"
class TextQuad : public UiQuad
{
public:
	TextQuad(glm::vec2 bottomLeft, glm::vec2 topRight);

	void Draw() override;
protected:
	std::string text = "Testing";
	float textureColour;

};