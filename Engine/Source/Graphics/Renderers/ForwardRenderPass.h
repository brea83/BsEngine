#pragma once
#include "RenderPass.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/MaterialInstance.h"
#include <glm/glm.hpp>


#define MAX_LIGHTS 6

namespace Pixie
{
	class Shader;
	class Texture;

	class ForwardRenderPass : public RenderPass
	{
	public:
		ForwardRenderPass();
		~ForwardRenderPass();
		// Inherited via RenderPass
		void Execute(Scene& sceneToRender) override;
	private:
		std::shared_ptr<Shader> m_Shader;
		MaterialInstance m_FallbackMaterial{};

		void SendLightsToShader(entt::registry& registry);
	};

}