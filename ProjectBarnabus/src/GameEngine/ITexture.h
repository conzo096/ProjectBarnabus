#pragma once
#include "stb_image.h"
#include <string>
class ITexture
{
public:
	~ITexture()
	{
	};
public:
	virtual bool LoadTexture(const std::string& file) = 0;
	
	unsigned int GetTextureId()
	{
		return textureId;
	}

	int GetWidth() 
	{
		return width; 
	}
	
	int GetHeight()
	{
		return height;
	}

protected:
	unsigned int textureId;
	int width, height;
};