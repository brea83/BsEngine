#pragma once
#include <glm/glm.hpp>

namespace Pixie
{
	struct FrameBufferSpecification
	{
		uint32_t Width{ 1280 };
		uint32_t Height{ 720 };
		uint32_t Samples{ 1 };

		bool SwapChainTarget{ false };
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferSpecification& specification);
		virtual ~FrameBuffer();

		const FrameBufferSpecification& GetSpecification() { return  m_Specification; }
		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& specification);

		// if need a new size invalidate and recreate with new size
		void Bind();
		void UnBind();

		uint32_t GetColorAttachmentRendererId() const { return m_ColorAttatchment; }

		void Resize();
		void Resize(uint32_t width, uint32_t height);
	private:
		uint32_t m_RendererId{ 0 };
		uint32_t m_ColorAttatchment{ 0 };
		uint32_t m_DepthAttachment{ 0 };
		FrameBufferSpecification m_Specification;
	};

}