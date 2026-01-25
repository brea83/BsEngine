#pragma once
#include <glm/glm.hpp>

namespace Pixie
{
	// ToDo: make FrameBuffers more generic and able to have variable num and types of attatchments

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
		//FrameBuffer(const FrameBufferSpecification& specification);
		virtual ~FrameBuffer() { }

		virtual const FrameBufferSpecification& GetSpecification() = 0;//{ return  m_Specification; }

		//static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& specification);

		// if need a new size invalidate and recreate with new size
		virtual void Bind() = 0;;
		virtual void UnBind() = 0;

		uint32_t GetFrameBufferID() const { return m_FrameBufferID; }
		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0; // { return m_ColorAttatchment; }
		uint32_t GetDepthAttatchmentID() const { return m_DepthAttachment; }

		virtual void Resize() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
	protected:
		uint32_t m_FrameBufferID{ 0 };
		//uint32_t m_ColorAttatchment{ 0 };
		uint32_t m_DepthAttachment{ 0 };
		//FrameBufferSpecification m_Specification;
		void PrintFrameBufferError(GLenum statusResult);
	};

	class OldFrameBuffer : public FrameBuffer
	{
	public:
		OldFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OldFrameBuffer();

		virtual const FrameBufferSpecification& GetSpecification() override { return  m_Specification; }
		static std::shared_ptr<OldFrameBuffer> Create(const FrameBufferSpecification& specification);

		// if need a new size invalidate and recreate with new size
		virtual void Bind() override;
		virtual void UnBind() override;

		//uint32_t GetFrameBufferID() const { return m_FrameBufferID; }
		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override { return m_ColorAttatchment; }
		//uint32_t GetDepthAttatchmentID() const { return m_DepthAttachment; }

		void Resize();
		void Resize(uint32_t width, uint32_t height);
	private:
		//uint32_t m_FrameBufferID{ 0 };
		uint32_t m_ColorAttatchment{ 0 };
		//uint32_t m_DepthAttachment{ 0 };
		FrameBufferSpecification m_Specification;
	};

}