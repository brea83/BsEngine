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
		//glm::mat4 lightProjection = glm::mat4();
		//glm::mat4 lightView = glm::mat4();
		float near_plane = 1.0;
		float far_plane = 7.5f;

		if (mainLight)
		{
			// light direction is used as position with an ortho projection
			LightComponent& light = mainLight.GetComponent<LightComponent>();
			glm::vec3 lightPos = -light.Direction;
			/*glm::vec3 lightRot = glm::radians(glm::vec3 (45.0f, -45.0f, 0.0f ));
			glm::vec3 direction;
			direction.x = cos(lightRot.x * cos(lightRot.y));
			direction.y = sin(lightRot.y);
			direction.z = sin(lightRot.x) * cos(lightRot.y);
			glm::vec3 forward = glm::normalize(direction);
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 up = glm::normalize(glm::cross(right, forward));*/


			glm::vec2 viewportSize = EngineContext::GetEngine()->GetViewportSize();
			float aspectRatio = viewportSize.x / viewportSize.y;
			

			// get the light's projection and view to create a light-space matrix
			float zoom = 1.0f;
			m_LightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			m_LightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = m_LightProjection * m_LightView;

			bUseLightMatrix = true;
		}


		uint32_t prevPassDepth{ 0 };
		uint32_t prevPassColor{ 0 };

		for (size_t i = 0; i < m_Passes.size(); i++)
		{
			
			std::shared_ptr<Shader> shader = m_Passes[i]->GetShader();
			shader->SetUniformBool("bUseShadowMap", bUseLightMatrix);
			if (bUseLightMatrix)
			{
				shader->SetUniformMat4("lightViewMat", m_LightView);
				shader->SetUniformMat4("lightProjMat", m_LightProjection);
				shader->SetUniformFloat("lightNearPlane", near_plane);
				shader->SetUniformFloat("lightFarPlane", far_plane);
			}

			m_Passes[i]->Execute(scene, prevPassDepth, prevPassColor);

			prevPassDepth = m_Passes[i]->GetDepthAttatchmentID();
			prevPassColor = m_Passes[i]->GetColorAttatchmentID();
			
			std::shared_ptr<FrameBuffer> passBuffer = m_Passes[i]->GetFrameBuffer();
			if (passBuffer != nullptr)
				m_FrameBuffer->Bind();
		}
	}

	void ForwardRenderer::EndFrame(Scene& scene)
	{
		m_FrameBuffer->UnBind();
	}
}