#pragma once
#include <glm/vec3.hpp>
#include "Scene/GameObject.h"
#include <Pixie.h>

namespace Pixie
{
    template <typename T>
    void DrawComponent(Pixie::GameObject& selected);

    class Scene;
    class DetailsViewPanel
    {
    public:
        static bool Draw(std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> selected);

    protected:
       // std::string_view m_SelectedName;
    
        static bool ItterateThroughComponents(std::shared_ptr<Scene> scene, GameObject& selected);
        static void DrawComponents(std::shared_ptr<Scene> scene, GameObject& selected/*std::unordered_map<size_t, std::shared_ptr<Component>>& componentMap*/);

        static void DrawOrthographicCamProps(Camera& camera);
        static void DrawPerspectiveCamProps(Camera& camera);

    };
}