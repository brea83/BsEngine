#pragma once
#include "Renderer.h"
#include "Graphics/UniformBuffer.h"

namespace Pixie
{
	class TransformComponent;
	class Camera;
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer(/*EngineContext* engineContext*/);
		// Inherited via Renderer
		void Init() override;
		void BeginFrame(Scene& scene) override;
		void RenderFrame(Scene& scene) override;
		void EndFrame(Scene& scene) override;

		virtual std::unordered_map<std::string, std::shared_ptr<FrameBuffer>> GetAllRenderBuffers() override;
	protected:
		std::shared_ptr<Camera> m_LightCamera{ nullptr };
		std::shared_ptr<TransformComponent> m_LightTransfrom{ nullptr };
		glm::mat4 m_LightProjection{glm::mat4(1)};
		glm::mat4 m_LightView{glm::mat4(1)};
		UniformBuffer m_LightProjectionUBO;
		UniformBuffer m_CameraBlockUBO;

		glm::vec4 m_ShadowCascadeLevels;

		struct LightProjUboData
		{
			float farPlane; // alignment 0 and takes up 4 
			//glm::vec4 mainLightPosition; //alignment 4
			glm::vec4 cascadePlaneDistances;// [16] in shader;
			std::vector<glm::mat4> lightSpaceMatrices; // [16] in shader ;
		};

		struct CameraBlockData
		{
			glm::mat4 view;
			glm::mat4 projection;
			glm::vec4 cameraPosition;
		};

		std::vector<glm::vec4> GetFrustumCornersWS(const glm::mat4& projection, const glm::mat4& view);
		glm::vec3 GetFrustumCenter(const std::vector<glm::vec4>& frustumCorners);
		std::vector<glm::mat4> GetLightSpaceMatrices(GameObject& mainLight, Camera& camera);
		glm::mat4 GetLightSpaceMatrix(GameObject& mainLight, Camera& cameraSegment);
	};

}