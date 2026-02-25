#include "BsPrecompileHeader.h"
#include "WireframePass.h"
#include "Graphics/Primitives/Cube.h"
#include "Scene/Components/Collider.h"
#include "Core.h"

namespace Pixie
{
	WireframePass::WireframePass()
	{
		// set up shader
		m_Shader = AssetLoader::LoadShader("../Assets/Shaders/WireframeVertex.glsl", "../Assets/Shaders/WireframeFrag.glsl");
		m_CubeNDC = std::make_shared<Cube>(true);
		m_UnitCube = AssetLoader::LoadPrimitive(PrimitiveMeshType::Cube);

		for (Mesh::Vertex& vertex : m_CubeNDC->m_Vertices)
		{
			vertex.Position *= 2.0f;
			vertex.Color = glm::vec3(1.0f);
		}
		m_CubeNDC->Init();
		//m_SpherePrimitive = AssetLoader::LoadPrimitive(PrimitiveMeshType::Sphere);

	}
	WireframePass::~WireframePass()
	{}
	void WireframePass::Execute(std::shared_ptr<Scene> sceneToRender, uint32_t prevPassDepthID, uint32_t prevPassColorID)
	{
		// editor only wireframes
		if (sceneToRender == nullptr || sceneToRender->GetSceneState() != SceneState::Edit)
			return;
		m_Shader->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// get scene registry for lighting and renderables
		entt::registry& registry = sceneToRender->GetRegistry();

		DrawFrustums(registry);
		DrawCubeColliders(registry);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_Shader->EndUse();
	}

	void WireframePass::DrawFrustums(entt::registry& registry)
	{
		auto group = registry.group<CameraComponent>(entt::get<TransformComponent>);

		m_Shader->SetUniformBool("BIsDrawingFrustum", true);
		m_Shader->SetUniformVec4("BaseColor", m_FrustumColor);


		for (auto entity : group)
		{
			CameraComponent& camera = group.get<CameraComponent>(entity);
			if (camera.IsActive)
				continue;
			glm::mat4 viewMatrix = glm::inverse(group.get<TransformComponent>(entity).GetModelMatrix());
			//m_Shader->SetUniformMat4("Transform", transformMatrix);
			glm::mat4 frustMatrix = glm::inverse(camera.Cam.ProjectionMatrix() * viewMatrix);
			m_Shader->SetUniformMat4("Transform", frustMatrix);

			m_CubeNDC->Render(*m_Shader);
		}
	}

	void WireframePass::DrawCubeColliders(entt::registry& registry)
	{
		auto view = registry.view<CubeCollider>();
		if (view.empty()) return;

		glm::vec4 baseColor = PIXIE_COLOR_COLLIDER;
		glm::vec4 collidingColor = PIXIE_COLOR_COLLIDER_HIT;
		m_Shader->SetUniformBool("BIsDrawingFrustum", false);
		m_Shader->SetUniformVec4("BaseColor", baseColor);

		bool previousColliderWasColliding = false;
		for (auto&& [entity, collider, transform] : registry.view<CubeCollider, TransformComponent>().each())
		{
			glm::vec3 scaledExtents = collider.Extents * transform.GetScale();
			glm::mat4 newScale = glm::scale(glm::mat4(1.0f), scaledExtents);
			glm::mat4 unscaledTransform = transform.GetUnscaledModelMatrix();

			glm::mat4 newTransform = unscaledTransform * newScale;
			m_Shader->SetUniformMat4("Transform", newTransform);
			if (collider.Colliding && !previousColliderWasColliding)
			{
				m_Shader->SetUniformVec4("BaseColor", collidingColor);
				previousColliderWasColliding = true;
			}
			else if (!collider.Colliding && previousColliderWasColliding)
			{
				m_Shader->SetUniformVec4("BaseColor", baseColor);
				previousColliderWasColliding = false;
			}

			m_CubeNDC->Render(*m_Shader);
		}
	}
	
}