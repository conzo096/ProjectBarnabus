#pragma once
#include "Texture.h"
class HeightMapTexture : public Texture
{
public:
	HeightMapTexture(const std::string &filename) throw (...);
	~HeightMapTexture();
	unsigned char* GetData();
private:
	unsigned char *data;
};

