#include "Texture.h"
#include<iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "Image.h"


Texture::Texture()
{}

Texture::Texture(const std::string & filePath)
{
	glGenTextures(1, &TextureObject);
	glBindTexture(GL_TEXTURE_2D, TextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	StbImageData data;
	StbImageWrapper::LoadImage(filePath, data);

	if (!data.BLoadSuccess) return;

	glTexImage2D(GL_TEXTURE_2D, 0, data.Format, data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.StbData);
	glGenerateMipmap(GL_TEXTURE_2D);

	StbImageWrapper::FreeImageData(data);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, TextureObject);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
