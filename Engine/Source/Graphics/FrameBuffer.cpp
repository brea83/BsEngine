#include "BsPrecompileHeader.h"
#include "FrameBuffer.h"
#include <glad/glad.h>

FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
{
	Resize();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &_rendererId);
}

std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification)
{
	return std::make_shared<FrameBuffer>(specification);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize()
{
	glCreateFramebuffers(1, &_rendererId);
	glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);

	glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttatchment);
	glBindTexture(GL_TEXTURE_2D, _colorAttatchment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _specification.Width, _specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttatchment, 0);

	// ADD A DEPTH BUFFER
	glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachment);
	glBindTexture(GL_TEXTURE_2D, _depthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _specification.Width, _specification.Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment, 0);

	// CHECK TO SEE IF IT WORKS	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed resize framebuffer" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
