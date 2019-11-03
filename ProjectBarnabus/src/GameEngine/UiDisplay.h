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
	virtual void Draw();

protected:
	std::map<std::string, UiQuad*> uiElements;
};