#pragma once
#include <glm/glm.hpp>

namespace Pixie
{
	// ToDo: make FrameBuffers more generic and able to have variable num and types of attatchments


	enum class FrameBufferTextureFormat
	{
		None = 0,
		// color attatchments
		RGBA8,
		RedInteger,
		// depth and stencil
		Depth24,
		Depth24Stencil8,
		// defaults
		Depth = Depth24Stencil8,
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format, bool isArray = false)
			: TextureFormat(format), IsTextureArray(isArray){ }

		FrameBufferTextureFormat TextureFormat{ FrameBufferTextureFormat::None };
		bool IsTextureArray{ false };
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments) { }

		std::vector<FrameBufferTextureSpecification> Attachments;
	};
	
	struct FrameBufferSpecification
	{

		uint32_t Width{ 1280 };
		uint32_t Height{ 720 };
		uint32_t Samples{ 1 };
		FrameBufferAttachmentSpecification Attachments;

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

		uint32_t GetFrameBufferID() const { return m_FrameBufferID; }
		uint32_t GetFirstColorAttachmentID() const
		{
			if (m_ColorAttachments.size() > 0)
			{
				return m_ColorAttachments[0];
			}

			return 0;
		}
		uint32_t GetDepthAttatchmentID() const { return m_DepthAttachment; }

		void Resize();
		void Resize(uint32_t width, uint32_t height);

		void ReadPixel(uint32_t attachmentIndex, int x, int y);
		void ClearAttachment(uint32_t attachmentIndex, int value);


	private:
		uint32_t m_FrameBufferID{ 0 };
		FrameBufferSpecification m_Specification;
		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification{ FrameBufferTextureFormat::None };

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment{ 0 };

		void AttachColorTextures();
	};

}