#pragma once
#include <string>
#include <map>

#include "UIQuad.h"

class UiDisplay
{
public:
	UiDisplay();
	~UiDisplay();

	void InitaliseAllQuads();
	void Draw();

protected:
	std::map<std::string, UiQuad*> uiElements;
};