#include "HeightMapTexture.h"
#include "GL\glew.h"
#include "stb_image.h"

HeightMapTexture::HeightMapTexture(const std::string &filename)
{
	glGenTextures(1, &textureId);

	int nrComponents;
	data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	filePath = filename;
	if (data)
	{
		GLenum format;
		//		 1           grey
		//       2           grey, alpha
		//       3           red, green, blue
		//       4           red, green, blue, alpha

		switch (nrComponents)
		{
		case 1:
			format = GL_LUMINANCE;
			break;
		case 2:
			format = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		stbi_image_free(data);
	}

}

HeightMapTexture::~HeightMapTexture()
{
	stbi_image_free(data);
}



