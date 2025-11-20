#pragma once
#include "BsPrecompileHeader.h"
#include <typeinfo>
class GameObject;

class Component
{
public:
    Component(GameObject* parent = nullptr, const std::string& name = "Component Name");

    virtual ~Component(){ }

    std::string Name;

    //might want to make an init that takes some kind of struct for data if what I'm seeing in zenith engine is accurate
    virtual void Initialize() = 0;

    virtual void CleanUp() = 0;
    virtual std::shared_ptr<Component> Clone() = 0;

    GameObject* AttatchedTo() const { return _parentObject; }
    virtual void SetParentObject(GameObject* newParent) = 0;

    bool operator ==(const Component& otherComponent)
    {
        return Name == otherComponent.Name 
            && typeid(*this) == typeid(otherComponent) 
            && _parentObject == otherComponent._parentObject;
    }

    virtual void OnUpdate() = 0;

protected:
    GameObject* _parentObject;

    // id generator

};


// components are hashed by their name and their typeid.hash_code()
struct ComponentHash
{
    std::size_t operator()(const Component& component) const
    {
        std::hash<std::string> NameHash;
        
        return NameHash(component.Name) ^ typeid(component).hash_code();
    }
};