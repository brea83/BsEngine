#pragma once
#include <string>


class Texture
{
public:
	Texture();
	Texture(const std::string& filePath);

	unsigned int TextureObject;

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;
};

