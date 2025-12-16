#pragma once
#include "Renderer.h"

namespace Pixie
{
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer(/*EngineContext* engineContext*/);
		// Inherited via Renderer
		void Init() override;
		void BeginFrame(Scene& scene) override;
		void RenderFrame(Scene& scene) override;
		void EndFrame(Scene& scene) override;
	};

}