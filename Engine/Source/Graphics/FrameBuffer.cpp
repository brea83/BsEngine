#include "BsPrecompileHeader.h"
#include "FrameBuffer.h"
#include <glad/glad.h>

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

		static void AttachColorTexture(bool isArray, uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(isArray), id, 0);
		}

		static void AttachDepthTexture(bool isArray, uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(isArray), id, 0);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::Depth24:
				return true;
			case FrameBufferTextureFormat::Depth24Stencil8:  
				return true;
			}

			return false;
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

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
		: m_Specification(specification)
	{
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

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		return std::make_shared<FrameBuffer>(specification);
	}

	void FrameBuffer::Bind()
	{
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize()
	{
		if (m_FrameBufferID)
		{
			glDeleteFramebuffers(1, &m_FrameBufferID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			AttachColorTextures();
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
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::Log(LOG_ERROR, "Failed create or resize framebuffer");
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Resize();
	}

	void FrameBuffer::AttachColorTextures()
	{
		for ( int i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
		{
			auto spec = m_ColorAttachmentSpecifications[i];
			uint32_t id{ 0 };
			Utils::CreateTextures(spec.IsTextureArray, &id, 1);
			Utils::BindTexture(spec.IsTextureArray, id);

			switch (spec.TextureFormat)
			{
			case FrameBufferTextureFormat::RGBA8:
				Utils::AttachColorTexture(spec.IsTextureArray, id, m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
				break;
			case FrameBufferTextureFormat::RedInteger:
				Utils::AttachColorTexture(spec.IsTextureArray, id, m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
				break;
			}

			m_ColorAttachments.push_back(id);
		}


	}
}