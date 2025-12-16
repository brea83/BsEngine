#pragma once
#include "Core.h"
#include "BsPrecompileHeader.h"
#include "Graphics/Texture.h"
#include "Editor/EditorCamera.h"
#include <EnTT/entt.hpp>


namespace Pixie
{
    class Component
    {
    public:
        virtual const std::string& Name() const = 0;
        virtual std::string ToString() const { return Name(); }

    };

    // components are hashed by their name and their typeid.hash_code()
    struct ComponentHash
    {
        std::size_t operator()(const Component& component) const
        {
            std::hash<std::string> NameHash;

            return NameHash(component.Name()) ^ typeid(component).hash_code();
        }
    };
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

    struct PointLight
    {   
        PointLight() = default;
        PointLight(const PointLight&) = default;
        // position should come from transform component
        //glm::vec3 Position{ 10.0f, 5.0f, -15.0f };
        glm::vec3 Color{ 1.0f, 1.0f, 0.95f };
        glm::vec3 Attenuations{ 1.0f, 0.01f, 0.00001f };
    };

    struct DirectionalLight
    {
        DirectionalLight() = default;
        DirectionalLight(const DirectionalLight&) = default;
        glm::vec3 Direction{ 0.5f, -0.5f, 0.5f };
        glm::vec3 Color{ 1.0f, 1.0f, 0.95f };
        glm::vec3 Attenuations{ 1.0f, 0.01f, 0.00001f };
    };
}