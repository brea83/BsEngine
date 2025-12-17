#pragma once
#include "RenderPass.h"
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
		std::shared_ptr<Texture> m_FallbackTexture;
		//todo: REPLACE THIS WITH PROPER MATERIALS SYSTEM
		std::shared_ptr<Texture> m_FallbackSpecularMap;

		void SendLightsToShader(Scene& currentScene);
	};

}