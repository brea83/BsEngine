#pragma once
#include <string>

namespace Pixie
{
    class GameObject;
    // Component bases for easier heirarchy drawing
    struct IComponentStruct
    {
        virtual std::string GetName() = 0;
        virtual void Draw(GameObject& selected) = 0;
        //virtual void Remove(GameObject& selected) = 0;
    };

    class IComponentClass
    {
    public:
        virtual std::string GetName() = 0;
        virtual void Draw(GameObject& selected) = 0;
        //virtual void Remove(GameObject& selected) = 0;
    };

}