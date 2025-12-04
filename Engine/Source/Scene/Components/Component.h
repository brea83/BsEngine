#pragma once
#include "BsPrecompileHeader.h"

#include "Editor/EditorCamera.h"

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