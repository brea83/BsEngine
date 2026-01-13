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
#include <glm/gtc/matrix_transform.hpp>

namespace Pixie
{
	ForwardRenderer::ForwardRenderer()
	{ }

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

		std::vector<uint32_t> shaderIDs;
		for (size_t i = 0; i < m_Passes.size(); i++)
		{
			shaderIDs.push_back(m_Passes[i]->GetShader()->ShaderProgram);
		}

		m_LightProjectionUBO = UniformBuffer(sizeof(LightProjUboData), GL_DYNAMIC_DRAW, "LightProjectionBlock", shaderIDs);

		m_GridShader = AssetLoader::LoadShader("../Assets/Shaders/GridVertex.glsl", "../Assets/Shaders/GridFragment.glsl");
		m_EditorGrid = AssetLoader::LoadMesh("../Assets/Meshes/DebugGrid.obj");

		if (m_GridShader == nullptr || m_EditorGrid == nullptr)
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
		glm::vec4 hypotheticalLightPos{ 1.0f };
	

		if (mainLight)
		{
			glm::vec2 viewportSize = EngineContext::GetEngine()->GetViewportSize();
			float aspectRatio = viewportSize.x / viewportSize.y;
			// light direction is used as position with an ortho projection
			LightComponent& light = mainLight.GetComponent<LightComponent>();
			TransformComponent lightTransform = mainLight.GetTransform();

			glm::vec3 forward = lightTransform.Forward();
			glm::vec3 forwardDegrees = glm::degrees(forward);

			hypotheticalLightPos = glm::vec4(-1.0f * forward, 1);
			m_LightTransfrom->SetPosition(hypotheticalLightPos);
			m_LightTransfrom->SetRotationEuler(lightTransform.GetRotationEuler());
			
			m_LightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
			m_LightView = glm::inverse(m_LightTransfrom->GetObjectToWorldMatrix());

			lightSpaceMatrix = m_LightProjection * m_LightView;

			m_LightProjectionUBO.Bind();
			m_LightProjectionUBO.UpdateVec4(0, hypotheticalLightPos);
			m_LightProjectionUBO.UpdateMat4(16, m_LightView);
			m_LightProjectionUBO.UpdateMat4(80, m_LightProjection);
			m_LightProjectionUBO.UnBind();

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
				// ToDo: move this data to a Uniform Buffer for better sharing across shaders
				/*shader->SetUniformVec3("mainLightPosition", hypotheticalLightPos);
				shader->SetUniformMat4("lightViewMat", m_LightView);
				shader->SetUniformMat4("lightProjMat", m_LightProjection);
				*/
			}

			m_Passes[i]->Execute(scene, prevPassDepth, prevPassColor);

			prevPassDepth = m_Passes[i]->GetDepthAttatchmentID();
			prevPassColor = m_Passes[i]->GetColorAttatchmentID();
			
			std::shared_ptr<FrameBuffer> passBuffer = m_Passes[i]->GetFrameBuffer();
			if (passBuffer != nullptr)
				m_FrameBuffer->Bind();
		}
		
		// post processing?

		if (m_DrawGridEnabled)
		{
			//TODO: set up uniform buffer for camera data to share across shaders
			GameObject cameraEntity = scene.GetActiveCameraGameObject();
			if (!cameraEntity) return; // no valid camera to render

			// set up rendering camera info
			TransformComponent& transform = cameraEntity.GetComponent<TransformComponent>();
			glm::vec3 camPosition = transform.GetPosition();

			glm::mat4 viewMatrix{ 1.0f };
			Camera* mainCam = scene.GetActiveCamera(viewMatrix);
			if (mainCam == nullptr)
			{
				Logger::Log(LOG_WARNING, "No Camera in the scene is set to active");
				return;
			}

			glm::mat4 projectionMatrix = mainCam->ProjectionMatrix();

			m_GridShader->Use();
			m_GridShader->SetUniformMat4("view", viewMatrix);
			m_GridShader->SetUniformMat4("projection", projectionMatrix);
			m_GridShader->SetUniformVec3("cameraPosition", camPosition);

			glm::mat4 transformMatrix = glm::mat4(1.0f); // send an identiy matrix, grid mesh is always at origin
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
			transformMatrix = transformMatrix * scale;
			m_GridShader->SetUniformMat4("transform", transformMatrix);

			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->GetDepthAttatchmentID());
			//m_GridShader->SetUniformInt("DepthMap", 0);

			m_EditorGrid->Render(*m_GridShader);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
		
	}

	

	void ForwardRenderer::EndFrame(Scene& scene)
	{
		m_FrameBuffer->UnBind();
	}
}