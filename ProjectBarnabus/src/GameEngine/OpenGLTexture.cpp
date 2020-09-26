#include "OpenGLTexture.h"
#include "BarnabusGameEngine.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <iostream>

OpenGLTexture::OpenGLTexture()
{
}

OpenGLTexture::OpenGLTexture(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
	glGenTextures(1, &textureId);
}

bool OpenGLTexture::LoadTexture(const std::string& file)
{
	filePath = file;
	
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
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Failed to load OpenGLTexture at: " + file, StringLog::Priority::Critical));
		return false;
	}

	stbi_image_free(data);

	BarnabusGameEngine::Get().AddMessageLog(StringLog("Created texture: " + filePath, StringLog::Priority::Low));

	return true;
}
