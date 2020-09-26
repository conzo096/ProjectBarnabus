#pragma once
#include "OpenGLTexture.h"
class HeightMapTexture : public OpenGLTexture
{
public:
	HeightMapTexture(const std::string &filename);
	~HeightMapTexture();
	unsigned char* GetData();
private:
	unsigned char *data;
};

