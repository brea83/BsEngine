#include "BsPrecompileHeader.h"
#include "ForwardRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ForwardRenderPass.h"
#include "EngineContext.h"
#include "GlfwWrapper.h"

ForwardRenderer::ForwardRenderer(/*EngineContext* engineContext*/)
{
	//m_Engine = engineContext;
	//Init();
}

void ForwardRenderer::Init()
{
	std::unique_ptr<ForwardRenderPass> pass = std::make_unique<ForwardRenderPass>();
	m_Passes.push_back(std::move(pass));

	FrameBufferSpecification frameBufferData;
	frameBufferData.Width = EngineContext::GetEngine()->GetWindow().WindowWidth();
	frameBufferData.Height = EngineContext::GetEngine()->GetWindow().WindowHeight();

	m_FrameBuffer = FrameBuffer::Create(frameBufferData);
	glEnable(GL_DEPTH_TEST);
}

void ForwardRenderer::BeginFrame(Scene& scene)
{
	m_FrameBuffer->Bind();
	glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ForwardRenderer::RenderFrame(Scene & scene)
{
	for (size_t i = 0; i < m_Passes.size(); i++)
	{
		m_Passes[i]->Execute(scene);
	}
}

void ForwardRenderer::EndFrame(Scene & scene)
{
	m_FrameBuffer->UnBind();
}
