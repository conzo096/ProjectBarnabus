#include "OpenGLHeightMapTexture.h"
#include "GL\glew.h"
#include "stb_image.h"
#include "BarnabusGameEngine.h"

HeightMapTexture::HeightMapTexture(const std::string &filename)
{
	filePath = filename;
	data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Creating height map texture: " + filename, StringLog::Priority::Low));
}

HeightMapTexture::~HeightMapTexture()
{
	stbi_image_free(data);
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Freed height map texture: " + filePath, StringLog::Priority::Low));
}

unsigned char* HeightMapTexture::GetData()
{
	return data; 
}


