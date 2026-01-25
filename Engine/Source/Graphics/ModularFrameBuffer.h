#pragma once
#include "FrameBuffer.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>

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

	struct ModularFBTextureSpecification
	{
		ModularFBTextureSpecification() = default;
		ModularFBTextureSpecification(FrameBufferTextureFormat format, uint32_t countInArray = 0)
			: TextureFormat(format), ArraySize(countInArray){ }

		FrameBufferTextureFormat TextureFormat{ FrameBufferTextureFormat::None };
		uint32_t ArraySize{ 0 };

		std::string TextureFormatToString()
		{
			switch (TextureFormat)
			{
			case FrameBufferTextureFormat::None:
				return "None";
			case FrameBufferTextureFormat::RGBA8:
				return "RGBA8";
			case FrameBufferTextureFormat::RedInteger:
				return "Red Integer";
			case FrameBufferTextureFormat::Depth24:
				return "Depth24";
			case FrameBufferTextureFormat::Depth24Stencil8:
				return "Depth24Stencil8";
			default:
				return "ERROR";
			}
		}
	};

	struct ModularFBAttachmentSpecification
	{
		ModularFBAttachmentSpecification() = default;
		ModularFBAttachmentSpecification(std::initializer_list<ModularFBTextureSpecification> attachments)
			: Attachments(attachments) { }

		std::vector<ModularFBTextureSpecification> Attachments;
	};
	
	struct ModularFBSpecification
	{

		uint32_t Width{ 1280 };
		uint32_t Height{ 720 };
		uint32_t Samples{ 1 };
		ModularFBAttachmentSpecification Attachments;

		bool SwapChainTarget{ false };
	};

	class ModularFrameBuffer : public FrameBuffer
	{
	public:
		ModularFrameBuffer(const ModularFBSpecification& specification);
		virtual ~ModularFrameBuffer();

		const ModularFBSpecification& GetModularSpecification() { return  m_Specification; }
		virtual const FrameBufferSpecification& GetSpecification() override { return  m_OldSpecification; }
		static std::shared_ptr<ModularFrameBuffer> Create(const ModularFBSpecification& specification);

		// if need a new size invalidate and recreate with new size
		virtual void Bind() override;
		virtual void UnBind() override;

		//uint32_t GetFrameBufferID() const { return m_FrameBufferID; }
		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override
		{
			if (m_ColorAttachments.size() > index)
			{
				return m_ColorAttachments[index];
			}

			return 0;
		}
		std::vector<uint32_t> GetColorAttachments() { return m_ColorAttachments; }

		//uint32_t GetDepthAttatchmentID() const { return m_DepthAttachment; }
		bool IsDepthTexture3D() { return m_DepthAttachmentSpecification.ArraySize > 0; }
		std::vector<uint32_t> GetDepthLayerViews() { return m_DepthLayerViews; }

		virtual void Resize() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		void ReadPixel(uint32_t attachmentIndex, int x, int y);
		void ClearAttachment(uint32_t attachmentIndex, int value);


	protected:
		//uint32_t m_FrameBufferID{ 0 };
		ModularFBSpecification m_Specification;
		FrameBufferSpecification m_OldSpecification;
		std::vector<ModularFBTextureSpecification> m_ColorAttachmentSpecifications;
		ModularFBTextureSpecification m_DepthAttachmentSpecification{ FrameBufferTextureFormat::None };

		std::vector<uint32_t> m_ColorAttachments;
		//uint32_t m_DepthAttachment{ 0 };
		std::vector<uint32_t> m_DepthLayerViews;

		void AttachColorTextures();
	};

}