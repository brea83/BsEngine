#include "BsPrecompileHeader.h"
#include "StbImageWrapper.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pixie
{
	void StbImageWrapper::LoadImage(const std::string& filePath, StbImageData& imageData)
	{
		Logger::Log(LOG_TRACE, ":::::::::::::::::::::::::::::::::::::::::::::" );
		Logger::Log(LOG_TRACE, "stbi trying to load file path: {}", filePath);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &imageData.Width, &imageData.Height, &imageData.ChannelsCount, 0);

		if (data)
		{
			switch (imageData.ChannelsCount)
			{
			case 1:
				imageData.Format = GL_RED;
				break;
			case 3:
				imageData.Format = GL_RGB;
				break;
			case 4:
				imageData.Format = GL_RGBA;
				break;
			default:
				imageData.Format = GL_RGB;
				break;
			}
			imageData.StbData = data;
			imageData.BLoadSuccess = true;
			Logger::Log(LOG_TRACE, "FILE LOAD SUCCESS");
		}
		else
		{
			Logger::Log(LOG_ERROR, "FAILED TO LOAD TEXTURE: {}", filePath);
			imageData.BLoadSuccess = false;
		}

	}

	void StbImageWrapper::FreeImageData(StbImageData& imageData)
	{
		stbi_image_free(imageData.StbData);
	}
}