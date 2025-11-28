#pragma once
#include <glm/glm.hpp>

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

	const FrameBufferSpecification& GetSpecification() { return  _specification;  }
	static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& specification);

	// if need a new size invalidate and recreate with new size
	void Bind();
	void UnBind();

	uint32_t GetColorAttachmentRendererId() const { return _colorAttatchment; }

	void Resize();
	void Resize(uint32_t width, uint32_t height);
private:
	uint32_t _rendererId{ 0 };
	uint32_t _colorAttatchment{ 0 };
	uint32_t _depthAttachment{ 0 };
	FrameBufferSpecification _specification;
};

