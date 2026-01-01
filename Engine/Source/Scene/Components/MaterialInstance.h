#pragma once
#include "Scene/Components/Component.h"
#include "Resources/AssetLoader.h"

namespace Pixie
{
    struct MaterialInstance
    {
        MaterialInstance() = default;
        MaterialInstance(const MaterialInstance&) = default;

        static constexpr SerializableComponentID ID{ SerializableComponentID::MaterialInstance };
        std::string BaseMapPath{ "" };
        std::shared_ptr<Texture> BaseMap{ nullptr };

        std::string NormalMapPath{ "" };
        std::shared_ptr<Texture> NormalMap{ nullptr };

        std::string MetallicMapPath{ "" };
        std::shared_ptr<Texture> MetallicMap{ nullptr };
        std::string SpecularMapPath{ "" };
        std::shared_ptr<Texture> SpecularMap{ nullptr };
        bool BMapIsRoughness{ true };
        float AmbientMultiplier{ 1.0f };
        float Smoothness{ 0.3f };
        float SpecularPower{ 32.0f };

        static void Serialize(StreamWriter* stream, const MaterialInstance& component)
        {
            //stream->WriteRaw<SerializableComponentID>(component.ID);

            stream->WriteString(component.BaseMapPath);
            stream->WriteString(component.NormalMapPath);
            stream->WriteString(component.MetallicMapPath);
            stream->WriteString(component.SpecularMapPath);
            stream->WriteRaw<bool>(component.BMapIsRoughness);
            stream->WriteRaw<float>(component.AmbientMultiplier);
            stream->WriteRaw<float>(component.Smoothness);
            stream->WriteRaw<float>(component.SpecularPower);
        }
        static bool Deserialize(StreamReader* stream, MaterialInstance& component)
        {
           /* SerializableComponentID readID;
            stream->ReadRaw<SerializableComponentID>(readID);
            if (readID != component.ID) return false;*/

            stream->ReadString(component.BaseMapPath);
            stream->ReadString(component.NormalMapPath);
            stream->ReadString(component.MetallicMapPath);
            stream->ReadString(component.SpecularMapPath);
            stream->ReadRaw<bool>(component.BMapIsRoughness);
            stream->ReadRaw<float>(component.AmbientMultiplier);
            stream->ReadRaw<float>(component.Smoothness);
            stream->ReadRaw<float>(component.SpecularPower);
            return true;
        }

        bool operator== (const MaterialInstance& other) const
        {
            return other.BaseMapPath == BaseMapPath
                && other.NormalMapPath == NormalMapPath
                && other.MetallicMapPath == MetallicMapPath
                && other.SpecularMapPath == SpecularMapPath
                && other.AmbientMultiplier == AmbientMultiplier
                && other.Smoothness == Smoothness
                && other.SpecularPower == SpecularPower;
        }

        bool operator!=(const MaterialInstance& other) const
        {
            return !(*this == other);
        }

        void Reload()
        {
            std::shared_ptr<Texture> newColor = ReloadTexture(BaseMapPath, TextureType::Diffuse);
            if (newColor != nullptr)
            {
                BaseMap = newColor;
            }
            else
            {
                BaseMapPath = "";
            }

            std::shared_ptr<Texture> normal = ReloadTexture(NormalMapPath, TextureType::Normal);
            if (normal != nullptr)
            {
                NormalMap = normal;
            }
            else
            {
                NormalMapPath = "";
            }

            std::shared_ptr<Texture> metal = ReloadTexture(MetallicMapPath, TextureType::GltfMetalicRoughness);
            if (metal != nullptr)
            {
                MetallicMap = metal;
            }
            else
            {
                MetallicMapPath = "";
            }

            std::shared_ptr<Texture> specular = ReloadTexture(SpecularMapPath, TextureType::Specular);
            if (specular != nullptr)
            {
                SpecularMap = specular;
            }
            else
            {
                SpecularMapPath = "";
            }
        }

        std::shared_ptr<Texture> ReloadTexture(std::string path, TextureType type)
        {
            if (path.empty()) return nullptr;
            std::shared_ptr<Texture> newTexture = AssetLoader::LoadTexture(path, type);
            return newTexture;
        }
    };

}