#pragma once
#include "Renderer.h"

namespace Pixie
{
	class TransformComponent;
	class Camera;
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer(/*EngineContext* engineContext*/);
		// Inherited via Renderer
		void Init() override;
		void BeginFrame(Scene& scene) override;
		void RenderFrame(Scene& scene) override;
		void EndFrame(Scene& scene) override;
	protected:
		std::shared_ptr<Camera> m_LightCamera{ nullptr };
		std::shared_ptr<TransformComponent> m_LightTransfrom{ nullptr };
		glm::mat4 m_LightProjection{glm::mat4(1)};
		glm::mat4 m_LightView{glm::mat4(1)};
	};

}