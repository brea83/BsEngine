#pragma once
#include <vector>
#include <memory>
#include "Scene/Scene.h"
#include "RenderPass.h"
#include "Graphics/FrameBuffer.h"

class EngineContext;

class Renderer
{
public:
	virtual void Init() = 0;
	virtual void BeginFrame(Scene& scene) = 0;
	virtual void RenderFrame(Scene& scene) = 0;
	virtual void EndFrame(Scene& scene) = 0;

	std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return _frameBuffer; }
	virtual uint32_t GetFrameBufferID() { return _frameBuffer->GetColorAttachmentRendererId(); }
protected:
	EngineContext* _engine;
	std::vector<std::unique_ptr<RenderPass>> _passes;

	std::shared_ptr<FrameBuffer> _frameBuffer;
};

