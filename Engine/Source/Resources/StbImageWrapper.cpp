#include "BsPrecompileHeader.h"
#include "StbImageWrapper.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include <glad/glad.h>

namespace Pixie
{
	void StbImageWrapper::LoadImage(const std::string& filePath, StbImageData& imageData)
	{
		std::cout << ":::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
		std::cout << "trying to load file path: " << filePath << std::endl;

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
			std::cout << "FILE LOAD SUCCESS" << std::endl;
		}
		else
		{
			std::cerr << "Failed to load texture: " << filePath << std::endl;
			imageData.BLoadSuccess = false;
		}

	}

	void StbImageWrapper::FreeImageData(StbImageData& imageData)
	{
		stbi_image_free(imageData.StbData);
	}
}