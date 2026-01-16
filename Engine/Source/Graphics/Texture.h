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

	enum class MaterialTextureType
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

	const std::unordered_map<MaterialTextureType, std::string> TextureTypeToString = {
		{MaterialTextureType::Diffuse, "Diffuse"},
		{MaterialTextureType::Specular, "Specular"},
		{MaterialTextureType::Ambient, "Ambient"},
		{MaterialTextureType::Emmissive, "Emmissive"},
		{MaterialTextureType::Height, "Height"},
		{MaterialTextureType::Normal, "Normal"},
		{MaterialTextureType::Gloss, "Gloss"},
		{MaterialTextureType::Opacity, "Opacity"},
		{MaterialTextureType::Displacement, "Displacement"},
		{MaterialTextureType::Lightmap, "Lightmap"},
		{MaterialTextureType::Reflection, "Reflection"},
		//pbr pipeline types
		{MaterialTextureType::BaseColor, "BaseColor"},
		{MaterialTextureType::NormalCamera, "NormalCamera"},
		{MaterialTextureType::EmissionColor, "EmissionColor"},
		{MaterialTextureType::Metalness, "Metalness"},
		{MaterialTextureType::Roughness, "Roughness"},
		{MaterialTextureType::Ao, "Ao"},
		/*
		* gltf standard packing
		* red = Ao
		* greem = roughness
		* blue = metalness
		* some ppl use alpha for clipping but its not standard
		*/
		{MaterialTextureType::GltfMetalicRoughness, ""},
	};

	struct TextureSpecification
	{
		MaterialTextureType Type{ MaterialTextureType::Diffuse };
		Min_FilterType MinFilterType{ Min_FilterType::Nearest_Nearest };
		Mag_FilterType MagFilterType{ Mag_FilterType::Nearest };
	};

	class Texture : public Resource
	{
	public:

		Texture();
		Texture(StbImageData& data, MaterialTextureType type = MaterialTextureType::Diffuse);
		Texture(const std::string& filePath);

		unsigned int TextureObject{ 0 };
		static const char* Min_FilterModeNames[(unsigned long long)Min_FilterType::COUNT];
		static const char* Mag_FilterModeNames[(unsigned long long)Mag_FilterType::COUNT];
		MaterialTextureType Type{ MaterialTextureType::Diffuse };

		Min_FilterType GetMinFilterType() const { return m_MinFilterType; }
		Mag_FilterType GetMagFilterType() const { return m_MagFilterType; }
		static MaterialTextureType GetTypeByString(const std::string& searchString);

		void UpdateTextureFilters(const std::string& filePath, Min_FilterType minFilter, Mag_FilterType magFilter);
		void CreateTexture(const std::string& filePath, TextureSpecification specification);
		void Bind(unsigned int slot = 0) const;
		void UnBind() const;

	private:
		Min_FilterType m_MinFilterType{ Min_FilterType::Nearest_Nearest };
		Mag_FilterType m_MagFilterType{ Mag_FilterType::Nearest };

		int GetGlMin();
		int GetGlMag();
		GLenum GetGlEnumFromType(MaterialTextureType type, int channelCount);
		void CreateTexture(const std::string& filePath, bool overrideType = false, MaterialTextureType type = MaterialTextureType::Diffuse);
	};

}