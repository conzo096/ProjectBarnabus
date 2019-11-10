#include "Texture.h"
#include "BarnabusGameEngine.h"
 
#include <GL/glew.h>
#include <iostream>

Texture::Texture()
{
}

Texture::Texture(GLuint w, GLuint h)
{
	width = w;
	height = h;
	glGenTextures(1, &textureId);
	textureType = GL_TEXTURE_2D;
}

bool Texture::LoadTexture(std::string file)
{
	filePath = file;
	textureType = GL_TEXTURE_2D;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	// set the texture wrapping/filtering options
	// todo update this with flags to configure
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	GLenum format;
	switch (channels)
	{
	case 1:
		format = GL_LUMINANCE;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Failed to load Texture at: " + file, StringLog::Priority::Critical));
		return false;
	}
	stbi_image_free(data);

	return true;
}

unsigned int Texture::GetTextureId()
{
	return textureId;
}

int Texture::GetWidth()
{
	return width;
}

int Texture::GetHeight()
{
	return height;
}
