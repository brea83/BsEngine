#include "BsPrecompileHeader.h"
#include "ForwardRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ForwardRenderPass.h"
#include "ShadowPass.h"
#include "EngineContext.h"
#include "GlfwWrapper.h"
#include "Scene/GameObject.h"

namespace Pixie
{
	ForwardRenderer::ForwardRenderer(/*EngineContext* engineContext*/)
	{
		//m_Engine = engineContext;
		//Init();
	}

	void ForwardRenderer::Init()
	{
		std::unique_ptr<ShadowPass> shadows = std::make_unique<ShadowPass>();
		m_Passes.push_back(std::move(shadows));

		std::unique_ptr<ForwardRenderPass> pass = std::make_unique<ForwardRenderPass>();
		m_Passes.push_back(std::move(pass));

		FrameBufferSpecification frameBufferData;
		glm::vec2 viewportSize = EngineContext::GetEngine()->GetViewportSize();
		frameBufferData.Width = viewportSize.x;
		frameBufferData.Height = viewportSize.y;

		m_FrameBuffer = FrameBuffer::Create(frameBufferData);
		glEnable(GL_DEPTH_TEST);
	}

	void ForwardRenderer::BeginFrame(Scene& scene)
	{
		m_FrameBuffer->Bind();
		glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
	}

	void ForwardRenderer::RenderFrame(Scene& scene)
	{
		bool bUseLightMatrix{ false };
		GameObject mainLight = scene.GetMainLight();
		glm::mat4 lightSpaceMatrix = glm::mat4();
		float near_plane = 1.0f, far_plane = 7.5f;

		if (mainLight)
		{
			// light direction is used as position with an ortho projection
			LightComponent& light = mainLight.GetComponent<LightComponent>();
			glm::vec3 lightPos = -light.Direction;

			// get the light's projection and view to create a light-space matrix
			glm::mat4 lightProjection, lightView;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			bUseLightMatrix = true;
		}


		uint32_t prevPassDepth{ 0 };
		uint32_t prevPassColor{ 0 };

		for (size_t i = 0; i < m_Passes.size(); i++)
		{
			std::shared_ptr<FrameBuffer> passBuffer = m_Passes[i]->GetFrameBuffer();
			if (passBuffer != nullptr)
				passBuffer->Bind();
			else
				m_FrameBuffer->Bind();
			
			std::shared_ptr<Shader> shader = m_Passes[i]->GetShader();
			shader->SetUniformBool("bUseShadowMap", bUseLightMatrix);
			if (bUseLightMatrix)
			{
				shader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
				shader->SetUniformFloat("lightNearPlane", near_plane);
				shader->SetUniformFloat("lightFarPlane", far_plane);
			}

			m_Passes[i]->Execute(scene, prevPassDepth, prevPassColor);

			prevPassDepth = m_Passes[i]->GetDepthAttatchmentID();
			prevPassColor = m_Passes[i]->GetColorAttatchmentID();
		}
	}

	void ForwardRenderer::EndFrame(Scene& scene)
	{
		m_FrameBuffer->UnBind();
	}
}