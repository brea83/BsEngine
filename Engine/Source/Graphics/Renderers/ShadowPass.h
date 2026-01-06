#pragma once
#include "RenderPass.h"

namespace Pixie
{
	class FrameBuffer;
	class Shader;

	class ShadowPass : public RenderPass
	{
	public:
		ShadowPass();
		virtual ~ShadowPass();

		// Inherited via RenderPass
		void Execute(Scene& sceneToRender, uint32_t prevPassDepthID = 0, uint32_t prevPassColorID = 0) override;
		std::shared_ptr<FrameBuffer> GetFrameBuffer() const override { return m_FrameBuffer; }
		std::shared_ptr<Shader> GetShader() override { return m_Shader; }
		uint32_t GetFrameBufferID() override;
		uint32_t GetColorAttatchmentID() override;
		uint32_t GetDepthAttatchmentID() override;

	protected:
		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		std::shared_ptr<Shader> m_Shader;
	};
}
