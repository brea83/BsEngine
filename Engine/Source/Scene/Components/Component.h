#pragma once
#include "Core.h"
#include "BsPrecompileHeader.h"
#include "Graphics/Texture.h"
#include <glm/glm.hpp>
#include <EnTT/entt.hpp>
#include "Resources/FileStream.h"


namespace Pixie
{
    enum class SerializableComponentID
    {
        TagComponent,
        NameComponent,
        HeirarchyComponent,
        TransformComponent,
        MeshComponent,
        MaterialInstance,
        LightComponent,
        CameraComponent,
        CameraController
    };
    struct TagComponent
    {
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) { }

        static constexpr SerializableComponentID ID{ SerializableComponentID::TagComponent };
        std::string Tag{"Default Tag"};

        static void Serialize(StreamWriter* stream, const TagComponent& component)
        {
            stream->WriteRaw<SerializableComponentID>(component.ID);
            stream->WriteString(component.Tag);
        }
        static bool Deserialize(StreamReader* stream, TagComponent& component)
        {
            SerializableComponentID readID;
            stream->ReadRaw<SerializableComponentID>(readID);
            if (readID != component.ID) return false;

            stream->ReadString(component.Tag);
            return true;
        }
    };

    struct NameComponent
    {
        NameComponent() = default;
        NameComponent(const NameComponent&) = default;
        NameComponent(const std::string& name) : Name(name) {}

        static constexpr SerializableComponentID ID{ SerializableComponentID::NameComponent };
        std::string Name{ "Name" };

        static void Serialize(StreamWriter* stream, const NameComponent& component)
        {
            stream->WriteRaw<SerializableComponentID>(component.ID);
            stream->WriteString(component.Name);
        }
        static bool Deserialize(StreamReader* stream, NameComponent& component)
        {
            SerializableComponentID readID;
            stream->ReadRaw<SerializableComponentID>(readID);
            if (readID != component.ID) return false;

            stream->ReadString(component.Name);
            return true;
        }
    };

    struct HeirarchyComponent
    {
        HeirarchyComponent() = default;
        HeirarchyComponent(const HeirarchyComponent&) = default;

        static constexpr SerializableComponentID ID{ SerializableComponentID::HeirarchyComponent };
        entt::entity Parent{ entt::null };
        std::vector<entt::entity> Children{};

        static void Serialize(StreamWriter* stream, const HeirarchyComponent& component)
        {
            stream->WriteRaw<SerializableComponentID>(component.ID);
            stream->WriteRaw<entt::entity>(component.Parent);
            stream->WriteArray<entt::entity>(component.Children);
        }
        static bool Deserialize(StreamReader* stream, HeirarchyComponent& component)
        {
            SerializableComponentID readID;
            stream->ReadRaw<SerializableComponentID>(readID);
            if (readID != component.ID) return false;

            stream->ReadRaw<entt::entity>(component.Parent);
            stream->ReadArray<entt::entity>(component.Children);
            return true;
        }
    };

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
        float AmbientMultiplier{ 1.0f };
        float Smoothness{ 0.3f };
        float SpecularPower{ 32.0f };

        static void Serialize(StreamWriter* stream, const MaterialInstance& component)
        {
            stream->WriteRaw<SerializableComponentID>(component.ID);

            stream->WriteString(component.BaseMapPath);
            stream->WriteString(component.NormalMapPath);
            stream->WriteString(component.MetallicMapPath);
            stream->WriteString(component.SpecularMapPath);
            stream->WriteRaw<float>(component.AmbientMultiplier);
            stream->WriteRaw<float>(component.Smoothness);
            stream->WriteRaw<float>(component.SpecularPower);
        }
        static bool Deserialize(StreamReader* stream, MaterialInstance& component)
        {
            SerializableComponentID readID;
            stream->ReadRaw<SerializableComponentID>(readID);
            if (readID != component.ID) return false;

            stream->ReadString(component.BaseMapPath);
            stream->ReadString(component.NormalMapPath);
            stream->ReadString(component.MetallicMapPath); 
            stream->ReadString(component.SpecularMapPath);
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
    };



    enum LightType
    {
        Directional,
        Point,
        Spot,
        // COUNT used for imgui combo windows
        COUNT,
    };

    const std::unordered_map<LightType, std::string> LightTypeToString = {
        {LightType::Directional, "Directional Light"},
        {LightType::Point, "Point Light"},
        {LightType::Spot, "Spot Light"},
    };

    struct LightComponent
    {   
        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
        
        static constexpr SerializableComponentID ID{ SerializableComponentID::LightComponent };
        bool Enabled{ true };
        LightType Type{ Point };
        static const char* LightTypeNames[(unsigned long long)LightType::COUNT];

        glm::vec3 Direction{ 0.5f, -0.5f, 0.5f }; // consider if this should just be getting the Forward from the transform?
        glm::vec3 Color{ 1.0f, 1.0f, 0.95f };
        glm::vec3 Attenuation{ 1.0f, 0.045f, 0.0075f };

        //spotlight only
        float InnerRadius{ 12.5f };
        float OuterRadius{ 15.0f };

        static void Serialize(StreamWriter* stream, const LightComponent& component)
        {
            stream->WriteRaw<SerializableComponentID>(component.ID);

        }
        static bool Deserialize(StreamReader* stream, LightComponent& component)
        {
            SerializableComponentID readID;
            stream->ReadRaw<SerializableComponentID>(readID);
            if (readID != component.ID) return false;
            return true;
        }
    };

}