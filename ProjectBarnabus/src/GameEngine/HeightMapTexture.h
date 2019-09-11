#pragma once
#include "Texture.h"
class HeightMapTexture : public Texture
{
private:
	unsigned char *data;
public:
	HeightMapTexture(const std::string &filename) throw (...);
	~HeightMapTexture();


	unsigned char* GetData() { return data; }
};

