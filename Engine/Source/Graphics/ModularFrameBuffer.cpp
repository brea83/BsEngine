#include "BsPrecompileHeader.h"
#include "ModularFrameBuffer.h"
#include <glad/glad.h>
//	CRITICAL TODO: FIGURE OUT WHY TRYING TO CREATE TWO COLOR ATTACHMENTS WHEN ONLY ASKING FOR ONE

namespace Pixie
{
	namespace Utils
	{
		static GLenum TextureTarget(bool isTextureArray) // ToStudy: add parameter and logic here when ready to add multisample support
		{
			return isTextureArray ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool isArray, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(isArray), count, outID);
		}

		static void BindTexture(bool isArray, uint32_t id)
		{
			glBindTexture(TextureTarget(isArray), id);
		}

		static void AttachColorTexture(uint32_t id, uint32_t framebufferID, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			//glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTextureStorage2D(id, 1, format, width, height);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);*/
			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureStorage2D(id, 1, internalFormat, width, height);
			glNamedFramebufferTexture(framebufferID, GL_COLOR_ATTACHMENT0 + index, id, 0);
		}

		static void AttachDepthTexture(uint32_t arraySize, uint32_t id, uint32_t framebufferID, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool bIsArray = arraySize > 0;


			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			
			constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

			if (bIsArray)
			{
				glTextureStorage3D(id, 1, format, width, height, arraySize);
			}
			else
			{
				glTextureStorage2D(id, 1, format, width, height);
			}
			glNamedFramebufferTexture(framebufferID, attachmentType, id, 0);

			
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::Depth24:
				return true;
			case FrameBufferTextureFormat::Depth24Stencil8:  
				return true;
			default:
				return false;
			}

		}

		static GLenum HazelFBTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::RGBA8:       return GL_RGBA8;
				case FrameBufferTextureFormat::RedInteger: return GL_RED_INTEGER;
				//case FrameBufferTextureFormat::Depth24:       return GL_RGBA8;
				//case FrameBufferTextureFormat::Depth24Stencil8: return GL_RED_INTEGER;
			}

			return 0;
		}
	}

	ModularFrameBuffer::ModularFrameBuffer(const ModularFBSpecification& specification)
		: m_Specification(specification)
	{
		m_OldSpecification.Width = m_Specification.Width;
		m_OldSpecification.Height = m_Specification.Height;
		m_OldSpecification.Samples = m_Specification.Samples;
		m_OldSpecification.SwapChainTarget = m_Specification.SwapChainTarget;

		for (auto attachmentSpec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(attachmentSpec.TextureFormat))
			{
				m_ColorAttachmentSpecifications.emplace_back(attachmentSpec);
			}
			else
			{
				m_DepthAttachmentSpecification = attachmentSpec;
			}
		}

		Resize();
	}

	ModularFrameBuffer::~ModularFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	std::shared_ptr<ModularFrameBuffer> ModularFrameBuffer::Create(const ModularFBSpecification& specification)
	{
		return std::make_shared<ModularFrameBuffer>(specification);
	}

	void ModularFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ModularFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ModularFrameBuffer::Resize()
	{
		if (m_FrameBufferID)
		{
			glDeleteFramebuffers(1, &m_FrameBufferID);
			if(m_ColorAttachments.size())
				glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			if(m_DepthLayerViews.size())
				glDeleteTextures(m_DepthLayerViews.size(), m_DepthLayerViews.data());
			if(m_DepthAttachment)
				glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthLayerViews.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_FrameBufferID);
		//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			
			AttachColorTextures();
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
		{
			bool bIsArray = m_DepthAttachmentSpecification.ArraySize > 0;
			Utils::CreateTextures(bIsArray, &m_DepthAttachment, 1);
			//Utils::BindTexture(bIsArray, m_DepthAttachment);

			uint32_t arraySize = m_DepthAttachmentSpecification.ArraySize;
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FrameBufferTextureFormat::Depth24:
				Utils::AttachDepthTexture(arraySize, m_DepthAttachment, m_FrameBufferID, m_Specification.Samples, GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			case FrameBufferTextureFormat::Depth24Stencil8:
				Utils::AttachDepthTexture(arraySize, m_DepthAttachment, m_FrameBufferID, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}

			if (bIsArray)
			{

				// make views for debugging the texture array
				m_DepthLayerViews.resize(arraySize);
				glGenTextures(arraySize, m_DepthLayerViews.data());
				for (int32_t i = 0; i < arraySize; ++i)
				{
					glTextureView(m_DepthLayerViews[i], GL_TEXTURE_2D, m_DepthAttachment, GL_DEPTH_COMPONENT32F, 0, 1, i, 1);
				}
			}
			
		}

		if (m_ColorAttachments.size() > 1)
		{
			if (m_ColorAttachments.size() > 4)
				Logger::Log(LOG_WARNING, "more than 4 color attatchments created, but only 4 will be used by the frameBuffer");

			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttatchment);
		//glBindTexture(GL_TEXTURE_2D, m_ColorAttatchment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttatchment, 0);

		//// ADD A DEPTH BUFFER
		//glGenTextures(1, &m_DepthAttachment);
		//glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		////glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		//
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		//// Set the list of draw buffers.
		//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// CHECK TO SEE IF IT WORKS	
			//std::cerr << "framebuffer error\n";
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			GLenum status = glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER);
		if ( status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::string depthType = m_DepthAttachmentSpecification.TextureFormatToString();
			std::string colorTypes{ "" };

			for (auto spec : m_ColorAttachmentSpecifications)
			{
				colorTypes += spec.TextureFormatToString();
				colorTypes += ", ";
			}

			Logger::Log(LOG_ERROR, "Failed create a frame buffer with {} color attachments, types: {}. and {} type depth attachment", m_ColorAttachments.size(), colorTypes, depthType);
			PrintFrameBufferError(status);
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ModularFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;

		m_OldSpecification.Width = width;
		m_OldSpecification.Height = height;
		Resize();
	}

	void ModularFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{}

	void ModularFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{}

	void ModularFrameBuffer::AttachColorTextures()
	{
		m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

		for ( int i = 0; i < m_ColorAttachments.size(); i++)
		{
			auto spec = m_ColorAttachmentSpecifications[i];
			uint32_t id{ 0 };
			bool bIsArray = spec.ArraySize > 0;
			Utils::CreateTextures(bIsArray, &id, 1);
			//Utils::BindTexture(bIsArray, id);

			switch (spec.TextureFormat)
			{
			case FrameBufferTextureFormat::RGBA8:
				Utils::AttachColorTexture( id, m_FrameBufferID, m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
				break;
			case FrameBufferTextureFormat::RedInteger:
				Utils::AttachColorTexture( id, m_FrameBufferID, m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
				break;
			}

			m_ColorAttachments[i] = id;
		}


	}
}