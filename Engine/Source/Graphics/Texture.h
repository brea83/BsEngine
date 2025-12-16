#pragma once
#include "Resources/Resource.h"
#include <string>
#include "Resources/StbImageWrapper.h"

namespace Pixie
{
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

	const std::unordered_map<TextureType, std::string> TextureTypeToString = {
		{TextureType::Diffuse, "Diffuse"},
		{TextureType::Specular, "Specular"},
		{TextureType::Ambient, "Ambient"},
		{TextureType::Emmissive, "Emmissive"},
		{TextureType::Height, "Height"},
		{TextureType::Normal, "Normal"},
		{TextureType::Gloss, "Gloss"},
		{TextureType::Opacity, "Opacity"},
		{TextureType::Displacement, "Displacement"},
		{TextureType::Lightmap, "Lightmap"},
		{TextureType::Reflection, "Reflection"},
		//pbr pipeline types
		{TextureType::BaseColor, "BaseColor"},
		{TextureType::NormalCamera, "NormalCamera"},
		{TextureType::EmissionColor, "EmissionColor"},
		{TextureType::Metalness, "Metalness"},
		{TextureType::Roughness, "Roughness"},
		{TextureType::Ao, "Ao"},
		/*
		* gltf standard packing
		* red = Ao
		* greem = roughness
		* blue = metalness
		* some ppl use alpha for clipping but its not standard
		*/
		{TextureType::GltfMetalicRoughness, ""},
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
		TextureType Type{ TextureType::Diffuse };

		Min_FilterType GetMinFilterType() const { return m_MinFilterType; }
		Mag_FilterType GetMagFilterType() const { return m_MagFilterType; }
		static TextureType GetTypeByString(const std::string& searchString);

		void CreateTexture(const std::string& filePath, Min_FilterType minFilter, Mag_FilterType magFilter);
		void Bind(unsigned int slot = 0) const;
		void UnBind() const;

	private:
		Min_FilterType m_MinFilterType{ Min_FilterType::Nearest_Nearest };
		Mag_FilterType m_MagFilterType{ Mag_FilterType::Nearest };

		int GetGlMin();
		int GetGlMag();
		void CreateTexture(const std::string& filePath);
	};

}