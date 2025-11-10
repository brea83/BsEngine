#include "BsPrecompileHeader.h"
#include "ForwardRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ForwardRenderPass.h"
#include "../../EngineContext.h"
#include "GlfwWrapper.h"

ForwardRenderer::ForwardRenderer(/*EngineContext* engineContext*/)
{
	//_engine = engineContext;
	//Init();
}

void ForwardRenderer::Init()
{
	std::unique_ptr<ForwardRenderPass> pass = std::make_unique<ForwardRenderPass>();
	_passes.push_back(std::move(pass));

	FrameBufferSpecification frameBufferData;
	frameBufferData.Width = EngineContext::GetEngine()->GetWindow().WindowWidth();
	frameBufferData.Height = EngineContext::GetEngine()->GetWindow().WindowHeight();

	_frameBuffer = FrameBuffer::Create(frameBufferData);
	glEnable(GL_DEPTH_TEST);
}

void ForwardRenderer::BeginFrame(Scene& scene)
{
	_frameBuffer->Bind();
	glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ForwardRenderer::RenderFrame(Scene & scene)
{
	for (size_t i = 0; i < _passes.size(); i++)
	{
		_passes[i]->Execute(scene);
	}
}

void ForwardRenderer::EndFrame(Scene & scene)
{
	_frameBuffer->UnBind();
}
