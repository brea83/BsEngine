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

	std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return m_FrameBuffer; }
	virtual uint32_t GetFrameBufferID() { return m_FrameBuffer->GetColorAttachmentRendererId(); }
protected:
	EngineContext* m_Engine;
	std::vector<std::unique_ptr<RenderPass>> m_Passes;

	std::shared_ptr<FrameBuffer> m_FrameBuffer;
};

