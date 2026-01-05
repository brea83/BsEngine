#include "BsPrecompileHeader.h"
#include "FrameBuffer.h"
#include <glad/glad.h>

namespace Pixie
{
	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
	{
		Resize();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(1, &m_ColorAttatchment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		return std::make_shared<FrameBuffer>(specification);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize()
	{
		if (m_RendererId)
		{
			glDeleteFramebuffers(1, &m_RendererId);
			glDeleteTextures(1, &m_ColorAttatchment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttatchment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttatchment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttatchment, 0);

		// ADD A DEPTH BUFFER
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		// CHECK TO SEE IF IT WORKS	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::Log(LOG_ERROR, "Failed resize framebuffer");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Resize();
	}
}