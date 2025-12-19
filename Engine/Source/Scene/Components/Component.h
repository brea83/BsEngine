#pragma once
#include "Core.h"
#include "BsPrecompileHeader.h"
#include "Graphics/Texture.h"
#include "Editor/EditorCamera.h"
#include <EnTT/entt.hpp>


namespace Pixie
{
    //class Component
    //{
    //public:
    //    virtual const std::string& Name() const = 0;
    //    virtual std::string ToString() const { return Name(); }

    //};

    //// components are hashed by their name and their typeid.hash_code()
    //struct ComponentHash
    //{
    //    std::size_t operator()(const Component& component) const
    //    {
    //        std::hash<std::string> NameHash;

    //        return NameHash(component.Name()) ^ typeid(component).hash_code();
    //    }
    //};
    struct TagComponent
    {
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) { }

        std::string Tag{"Default Tag"};

        // Inherited via Component
        std::string Name{ "Tag Component" };
    };

    struct NameComponent
    {
        NameComponent() = default;
        NameComponent(const NameComponent&) = default;
        NameComponent(const std::string& name) : Name(name) {}

        // Inherited via Component
        std::string Name{ "Name" };
    };

    struct HeirarchyComponent
    {
        HeirarchyComponent() = default;
        HeirarchyComponent(const HeirarchyComponent&) = default;

        entt::entity Parent{ entt::null };
        std::vector<entt::entity> Children{};
    };

    struct MaterialInstance
    {
        MaterialInstance() = default;
        MaterialInstance(const MaterialInstance&) = default;

        std::string BaseMapPath{ "" };
        std::shared_ptr<Texture> BaseMap{ nullptr };

        std::string MetallicMapPath{ "" };
        std::shared_ptr<Texture> MetallicMap{ nullptr };
        float AmbientMultiplier{ 1.0f };
        float Smoothness{ 0.3 };
        float SpecularPower{ 32.0f };
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
        
        bool Enabled{ true };
        LightType Type{ Point };
        static const char* LightTypeNames[(unsigned long long)LightType::COUNT];

        glm::vec3 Direction{ 0.5f, -0.5f, 0.5f }; // consider if this should just be getting the Forward from the transform?
        glm::vec3 Color{ 1.0f, 1.0f, 0.95f };
        glm::vec3 Attenuation{ 1.0f, 0.045f, 0.0075f };

        //spotlight only
        float InnerRadius{ 12.5f };
        float OuterRadius{ 15.0f };

    };

}