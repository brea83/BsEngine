#pragma once
#include "BsPrecompileHeader.h"
#include <typeinfo>
#include "Editor/EditorCamera.h"
class GameObject;

class Component
{
public:
    Component();

    virtual ~Component(){ }

    virtual std::string Name() const = 0;
    virtual void SetName(const std::string& name) = 0;

    //might want to make an init that takes some kind of struct for data if what I'm seeing in zenith engine is accurate
    virtual void Initialize() = 0;

    virtual void CleanUp() = 0;
    virtual std::shared_ptr<Component> Clone() = 0;

    virtual GameObject* GetParentObject() const = 0;
    virtual void SetParentObject(GameObject* newParent) = 0;

    bool operator ==(const Component& otherComponent)
    {
        return Name() == otherComponent.Name()
            && typeid(*this) == typeid(otherComponent) 
            && GetParentObject() == otherComponent.GetParentObject();
    }

    virtual void OnUpdate() = 0;

protected:

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

struct NewCameraComponent
{
public:
    NewCameraComponent() = default;
    NewCameraComponent(const NewCameraComponent&) = default;
    
    EditorCamera Cam;
};