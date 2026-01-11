#include "BsPrecompileHeader.h"
#include "ForwardRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ForwardRenderPass.h"
#include "ShadowPass.h"
#include "EngineContext.h"
#include "GlfwWrapper.h"
#include "Scene/GameObject.h"
#include "Graphics/Camera.h"

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

		m_LightCamera = std::make_shared<Camera>(45.0f, 1.0f);
		m_LightCamera->SetOrthographic(true);
		m_LightCamera->SetZoom(20.0f);

		m_LightTransfrom = std::make_shared<TransformComponent>();

		m_GridShader = AssetLoader::LoadShader("../Assets/Shaders/GridVertex.glsl", "../Assets/Shaders/GridFragment.glsl");

		if (m_GridShader == nullptr)
			m_DrawGridEnabled = false;
		else
			m_DrawGridEnabled = true;

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
		glm::vec3 hypotheticalLightPos{ 1.0f };
		//glm::mat4 lightProjection = glm::mat4();
		//glm::mat4 lightView = glm::mat4();
		float near_plane = 1.0;
		float far_plane = 7.5f;

		if (mainLight)
		{
			glm::vec2 viewportSize = EngineContext::GetEngine()->GetViewportSize();
			float aspectRatio = viewportSize.x / viewportSize.y;
			// light direction is used as position with an ortho projection
			LightComponent& light = mainLight.GetComponent<LightComponent>();
			TransformComponent lightTransform = mainLight.GetTransform();
			//glm::vec3 lightPos = -light.Direction;
			//glm::vec3 cameraRot{90.0f, 0.0, 0.0};

			glm::vec3 forward = lightTransform.Forward();//glm::normalize(light.Direction);
			glm::vec3 forwardDegrees = glm::degrees(forward);

			hypotheticalLightPos = -1.0f * forward;
			m_LightTransfrom->SetPosition(hypotheticalLightPos);
			m_LightTransfrom->SetRotationEuler(glm::vec3(forwardDegrees.y, 180.0f + forwardDegrees.x, forwardDegrees.z), AngleType::Degrees);
			
			m_LightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
			m_LightView = glm::lookAt(hypotheticalLightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

			//m_LightProjection = CameraManager::GetProjectionOutView(*m_LightCamera, *m_LightTransfrom, m_LightView);
			//m_LightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = m_LightProjection * m_LightView;

			bUseLightMatrix = true;
		}


		uint32_t prevPassDepth{ 0 };
		uint32_t prevPassColor{ 0 };

		for (size_t i = 0; i < m_Passes.size(); i++)
		{
			
			std::shared_ptr<Shader> shader = m_Passes[i]->GetShader();
			shader->Use();
			shader->SetUniformBool("bUseShadowMap", bUseLightMatrix);
			if (bUseLightMatrix)
			{
				
				shader->SetUniformVec3("mainLightPosition", hypotheticalLightPos);
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