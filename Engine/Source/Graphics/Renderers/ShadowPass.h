#pragma once
#include "RenderPass.h"

namespace Pixie
{
	//class FrameBuffer;
	//class ModularFrameBuffer;
	class Shader;

	class ShadowPass : public RenderPass
	{
	public:
		ShadowPass();
		virtual ~ShadowPass();

		// Inherited via RenderPass
		void Execute(Scene& sceneToRender, uint32_t prevPassDepthID = 0, uint32_t prevPassColorID = 0) override;
		std::shared_ptr<ModularFrameBuffer> GetFrameBuffer() const override;
		std::shared_ptr<Shader> GetShader() override { return m_Shader; }
		uint32_t GetFrameBufferID() const override;
		uint32_t GetColorAttatchmentID() const override;
		uint32_t GetDepthAttatchmentID() const override;

		virtual bool IsLit() const override;
		virtual void ForceLightsOff(bool value) override { m_LightsForcedOff = value; };

	protected:
		std::shared_ptr<ModularFrameBuffer> m_FrameBuffer{ nullptr };
		std::shared_ptr<Shader> m_Shader{ nullptr };
		bool m_LightsForcedOff{ false };
	};
}
