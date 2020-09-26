#pragma once
#include "ITexture.h"

class OpenGLTexture : public ITexture
{
public:
	OpenGLTexture();
	OpenGLTexture(unsigned int w, unsigned int h);
	bool LoadTexture(const std::string& file) override;
		
protected: 
	std::string filePath;
	int channels;
};