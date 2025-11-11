#include "BsPrecompileHeader.h"
#include "Texture.h"


const char* Texture::Min_FilterModeNames[] = 
{
	"Nearest_Nearest",
	"Linear_Nearest",
	"Nearest_Linear",
	"Linear_Linear"
};

const char* Texture::Mag_FilterModeNames[] =
{
	"Nearest",
	"Linear"
};

Texture::Texture()
	: Resource(ResourceType::Texture)
{}

Texture::Texture(StbImageData & data)
	: Resource(ResourceType::Texture)
{
	int glMinFilter = GetGlMin();
	int glMagFilter = GetGlMag();
	glGenTextures(1, &TextureObject);
	glBindTexture(GL_TEXTURE_2D, TextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, data.Format, data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.StbData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const std::string & filePath)
	: Resource(ResourceType::Texture)
{
	CreateTexture(filePath);
}

void Texture::CreateTexture(const std::string& filePath, Min_FilterType minFilter, Mag_FilterType magFilter)
{
	_minFilterType = minFilter;
	_magFilterType = magFilter;
	CreateTexture(filePath);
}

int Texture::GetGlMin()
{
	switch (_minFilterType)
	{
	case Min_FilterType::Nearest_Nearest:
		return GL_NEAREST_MIPMAP_NEAREST;
	case Min_FilterType::Linear_Nearest:
		return GL_LINEAR_MIPMAP_NEAREST;
	case Min_FilterType::Nearest_Linear:
		return GL_NEAREST_MIPMAP_LINEAR;
	case Min_FilterType::Linear_Linear:
		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		return GL_NEAREST_MIPMAP_NEAREST;
	}
}

int Texture::GetGlMag()
{
	switch (_magFilterType)
	{
	case Mag_FilterType::Nearest:
		return GL_NEAREST;
	case Mag_FilterType::Linear:
		return GL_LINEAR;
	default:
		return GL_NEAREST;
	}
}

void Texture::CreateTexture(const std::string& filePath)
{
	int glMinFilter = GetGlMin();
	int glMagFilter = GetGlMag();

	glGenTextures(1, &TextureObject);
	glBindTexture(GL_TEXTURE_2D, TextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);

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
