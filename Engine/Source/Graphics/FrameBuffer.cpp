#include "BsPrecompileHeader.h"
#include "FrameBuffer.h"
#include <glad/glad.h>

namespace Pixie
{
	// base FrameBuffer object
	void FrameBuffer::PrintFrameBufferError(GLenum statusResult)
	{
		switch (statusResult)
		{
		case 0:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: ERROR", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_UNDEFINED:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: UNDEFINED", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: INCOMPLETE ATTACHMENT,  one or more attachment points are framebuffer incomplete", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: MISSING ATTACHMENT. No Textures Attached", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: INCOMPLETE DRAW BUFFER. an attachment returned GL_NONE but is a Draw attachment", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: INCOMPLETE READ BUFFER. an attachment returned GL_NONE but is a Read attachment", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: UNSUPPORTED internal formats of one or more attached textures is invalid", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: INCOMPLETE MULTISAMPLE", m_FrameBufferID);
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			Logger::Log(LOG_ERROR, "FrameBuffer ID {}: INCOMPLETE LAYER TARGETS", m_FrameBufferID);
			break;
		}
		default:
			break;
		}
	}

	// old non modular FrameBuffer object
	OldFrameBuffer::OldFrameBuffer(const FrameBufferSpecification& specification)
		: m_Specification(specification)
	{
		Resize();
	}

	OldFrameBuffer::~OldFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
		glDeleteTextures(1, &m_ColorAttatchment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	std::shared_ptr<OldFrameBuffer> OldFrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		return std::make_shared<OldFrameBuffer>(OldFrameBuffer(specification));
	}

	void OldFrameBuffer::Bind()
	{
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OldFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OldFrameBuffer::Resize()
	{
		if (m_FrameBufferID)
		{
			glDeleteFramebuffers(1, &m_FrameBufferID);
			glDeleteTextures(1, &m_ColorAttatchment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_FrameBufferID);
		//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttatchment);
		//glBindTexture(GL_TEXTURE_2D, m_ColorAttatchment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
		glTextureParameteri(m_ColorAttatchment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttatchment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttatchment, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ColorAttatchment, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(m_ColorAttatchment, 1, GL_RGBA, m_Specification.Width, m_Specification.Height);
		glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0, m_ColorAttatchment, 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttatchment, 0);

		// ADD A DEPTH BUFFER
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		//glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);*/
		glTextureParameteri(m_DepthAttachment, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_DepthAttachment, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_DepthAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(m_DepthAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

		glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH_COMPONENT24, m_Specification.Width, m_Specification.Height);
		glNamedFramebufferTexture(m_FrameBufferID, GL_DEPTH_ATTACHMENT, m_DepthAttachment, 0);
		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// CHECK TO SEE IF IT WORKS	
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		GLenum status = glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::Log(LOG_ERROR, "OldFrameBuffer Incomplete");
			PrintFrameBufferError(status);
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OldFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Resize();
	}
	
}