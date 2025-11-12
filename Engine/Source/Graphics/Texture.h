#pragma once
#include "Resource.h"
#include <string>
#include "StbImageWrapper.h"

enum class Min_FilterType
{
	Nearest_Nearest,
	Linear_Nearest,
	Nearest_Linear,
	Linear_Linear,
	COUNT
};

enum class Mag_FilterType
{
	Nearest,
	Linear,
	COUNT
};

enum class TextureType
{
	Diffuse,
	Specular,
	Ambient,
	Emmissive,
	Height,
	Normal,
	Gloss,
	Opacity,
	Displacement,
	Lightmap,
	Reflection,
	//pbr pipeline types
	BaseColor,
	NormalCamera,
	EmissionColor,
	Metalness,
	Roughness,
	Ao,
	/*
	* gltf standard packing
	* red = Ao
	* greem = roughness
	* blue = metalness
	* some ppl use alpha for clipping but its not standard
	*/
	GltfMetalicRoughness
};

class Texture : public Resource
{
public:
	Texture();
	Texture(StbImageData& data);
	Texture(const std::string& filePath);

	unsigned int TextureObject{ 0 };
	static const char* Min_FilterModeNames[(unsigned long long)Min_FilterType::COUNT];
	static const char* Mag_FilterModeNames[(unsigned long long)Mag_FilterType::COUNT];

	Min_FilterType GetMinFilterType() const { return _minFilterType; }
	Mag_FilterType GetMagFilterType() const { return _magFilterType; }

	void CreateTexture(const std::string& filePath, Min_FilterType minFilter, Mag_FilterType magFilter);
	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

private:
	Min_FilterType _minFilterType{ Min_FilterType::Nearest_Nearest };
	Mag_FilterType _magFilterType{ Mag_FilterType::Nearest };

	int GetGlMin();
	int GetGlMag();
	void CreateTexture(const std::string& filePath);
};

