#pragma once
#include <glm/vec3.hpp>
#include "Scene/GameObject.h"

namespace Pixie
{
    class Scene;
    class Component;

    class DetailsViewPanel
    {
    public:
        static bool Draw(Scene* scene, GameObject& selcected);

    protected:
        ///Returns true if one of the vector values is changed
        static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 5.0f);

        static bool DrawStringProperty(const std::string& label, std::string& value, std::string& editingValue, bool& bIsEditing, float columnWidth = 10.0f);

        static void DrawComponents(Scene* scene, GameObject& selected/*std::unordered_map<size_t, std::shared_ptr<Component>>& componentMap*/);

    };
}