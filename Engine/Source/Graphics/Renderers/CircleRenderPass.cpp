#include "BsPrecompileHeader.h"
#include "CircleRenderPass.h"
#include "Graphics/Primitives/CircleMesh.h"

namespace Pixie
{
    CircleRenderPass::CircleRenderPass()
    {
        m_Shader = AssetLoader::LoadShader("../Assets/Shaders/CircleVertex.glsl", "../Assets/Shaders/CircleFragment.glsl");
    }
    void CircleRenderPass::Execute(Scene& sceneToRender, uint32_t prevPassDepthID, uint32_t prevPassColorID)
    {
        m_Shader->Use();
        // get scene registry for renderables
        entt::registry& registry = sceneToRender.GetRegistry();

        auto group = registry.group<CircleRendererComponent>(entt::get<TransformComponent>);

        //CircleUniforms uniforms; TODO figure out sorting components by matching uniform sets
        for (auto entity : group)
        {
            TransformComponent& transform = group.get<TransformComponent>(entity);
            CircleRendererComponent& circle = group.get<CircleRendererComponent>(entity);

            //std::shared_ptr<CircleMesh> mesh = std::dynamic_pointer_cast<CircleMesh>(circle.MeshResource);

            m_Shader->SetUniformMat4("Transform", transform.GetObjectToWorldMatrix());


            m_Shader->SetUniformVec4("Color", circle.Color);
            m_Shader->SetUniformFloat("Radius", circle.Radius);
            m_Shader->SetUniformFloat("LineWidth", circle.LineWidth);
            m_Shader->SetUniformFloat("Fade", circle.Fade);
            
            //mesh->Render(*m_Shader);

            circle.MeshResource->Render(*m_Shader);
        }

        m_Shader->EndUse();
    }

}