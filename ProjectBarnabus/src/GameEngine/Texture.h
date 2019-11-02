#pragma once
#include "stb_image.h"
#include <string>
#include <GL/glew.h>
class Texture
{
public:
	Texture();
	Texture(GLuint w, GLuint h);
	bool LoadTexture(std::string file);
	unsigned int GetTextureId();
	int GetWidth();
	int GetHeight();
		
protected: 
	std::string filePath;
	unsigned int textureId;
	int width, height, channels;
	GLenum textureType;
};