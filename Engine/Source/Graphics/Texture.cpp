#include "BsPrecompileHeader.h"
#include "Texture.h"

namespace Pixie
{
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

	Texture::Texture(StbImageData& data, MaterialTextureType type)
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

		Type = type;
		GLenum internalFormat = GetGlEnumFromType(Type, data.ChannelsCount);


		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.StbData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture::Texture(const std::string& filePath)
		: Resource(ResourceType::Texture)
	{
		CreateTexture(filePath);
	}

	MaterialTextureType Texture::GetTypeByString(const std::string& searchString)
	{
		//if(searchString == )
		return MaterialTextureType();
	}

	void Texture::UpdateTextureFilters(const std::string& filePath, Min_FilterType minFilter, Mag_FilterType magFilter)
	{
		m_MinFilterType = minFilter;
		m_MagFilterType = magFilter;
		CreateTexture(filePath);
	}

	void Texture::CreateTexture(const std::string& filePath, TextureSpecification specification)
	{
		m_MinFilterType = specification.MinFilterType;
		m_MagFilterType = specification.MagFilterType;
		CreateTexture(filePath);
	}

	int Texture::GetGlMin()
	{
		switch (m_MinFilterType)
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
		switch (m_MagFilterType)
		{
		case Mag_FilterType::Nearest:
			return GL_NEAREST;
		case Mag_FilterType::Linear:
			return GL_LINEAR;
		default:
			return GL_NEAREST;
		}
	}

	GLenum Texture::GetGlEnumFromType(MaterialTextureType type, int channelcount)
	{

		if (channelcount == 1) return GL_RED;

		if (type != MaterialTextureType::Diffuse)
		{
			switch (channelcount)
			{
			case 3:
				return  GL_RGB;
			case 4:
				return  GL_RGBA;
			default:
				return  GL_RGB;
			}
		}


		switch (channelcount)
		{
		case 3:
			return GL_SRGB;
		case 4:
			return GL_SRGB_ALPHA;
		default:
			return GL_SRGB;
		}
	}

	void Texture::CreateTexture(const std::string& filePath, bool overrideType, MaterialTextureType type)
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

		GLenum internalFormat;
		if (overrideType)
		{
			internalFormat = GetGlEnumFromType(type, data.ChannelsCount);
		}
		else
		{
			internalFormat = GetGlEnumFromType(Type, data.ChannelsCount);
		}
		

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.StbData);

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
}