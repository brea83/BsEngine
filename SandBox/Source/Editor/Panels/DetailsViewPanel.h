#pragma once
#include <glm/vec3.hpp>
#include "Scene/GameObject.h"
#include <Pixie.h>

namespace Pixie
{
    class Scene;

    class DetailsViewPanel
    {
    public:
        static bool Draw(Scene* scene, std::shared_ptr<GameObject> selected);

    protected:
       // std::string_view m_SelectedName;
        ///Returns true if one of the vector values is changed
        struct SliderParams
        {
            float ResetValue{ 0.0f };
            float Speed{ 0.1f };
            float Min{ FLT_MIN };
            float Max{ FLT_MAX };
            std::string Format{ "%.3f" };
            int Flags{ 0 };
        };
        static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 5.0f);
        static bool DrawVec2Control(const std::string& label, glm::vec2& values, glm::vec2 resetValues = glm::vec2(0.0f), float columnWidth = 5.0f);
        static bool DrawFloatControl(const std::string& label, float& value, SliderParams params, float columnWidth = 5.0f);

        static bool DrawStringProperty(const std::string& label, std::string& value, std::string& editingValue, float columnWidth = 10.0f);

        static void DrawComponents(Scene* scene, GameObject& selected/*std::unordered_map<size_t, std::shared_ptr<Component>>& componentMap*/);

        static void DrawOrthographicCamProps(Camera& camera);
        static void DrawPerspectiveCamProps(Camera& camera);

        static bool FileProperty(const std::string& label, std::string& value, const char* filter, float columnWidth = 10.0f);
    };
}