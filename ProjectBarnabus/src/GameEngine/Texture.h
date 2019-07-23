#pragma once
#include "stb_image.h"
#include <string>

class Texture
{
public:
	bool LoadTexture(std::string file);
	unsigned int GetTextureId();
private: 
	std::string filePath;
	unsigned int textureId;
	int width, height, channels;
};