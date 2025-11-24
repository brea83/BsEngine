#pragma once
#include "ImGuiPanel.h"
#include <glm/vec3.hpp>

class Scene;
class GameObject;
class Component;

class DetailsViewPanel 
{
public:
    static bool Draw(Scene* _currentScene = nullptr, int _selected = 0) ;

protected:
    ///Returns true if one of the vector values is changed
    static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 5.0f);
    static bool DrawStringProperty(const std::string& label, std::string& value, bool& bIsEditing, float columnWidth = 5.0f);

    static void DrawComponents(GameObject* selectedObject/*std::unordered_map<size_t, std::shared_ptr<Component>>& componentMap*/);

};

