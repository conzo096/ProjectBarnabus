#include "HeightMapTexture.h"
#include "GL\glew.h"
#include "stb_image.h"

HeightMapTexture::HeightMapTexture(const std::string &filename)
{
	data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
}

HeightMapTexture::~HeightMapTexture()
{
	stbi_image_free(data);
}



