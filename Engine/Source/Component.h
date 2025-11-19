#pragma once
#include "BsPrecompileHeader.h"
class GameObject;

class Component
{
public:
    virtual ~Component(){ }

    std::string Name;

    //might want to make an init that takes some kind of struct for data if what I'm seeing in zenith engine is accurate
    virtual void Initialize() = 0;

    virtual void CleanUp() = 0;
    virtual std::shared_ptr<Component> Clone() = 0;

    GameObject* AttatchedTo() { return _parentObject; }
    virtual void SetParentObject(GameObject* newParent) = 0;

protected:
	GameObject* _parentObject;

    // id generator

};

