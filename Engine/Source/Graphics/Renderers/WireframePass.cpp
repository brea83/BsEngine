#include "BsPrecompileHeader.h"
#include "WireframePass.h"
#include "Graphics/Primitives/Cube.h"

namespace Pixie
{
	WireframePass::WireframePass()
	{
		// set up shader
		m_Shader = AssetLoader::LoadShader("../Assets/Shaders/WireframeVertex.glsl", "../Assets/Shaders/WireframeFrag.glsl");
		m_CubePrimitive = std::make_shared<Cube>(true);//AssetLoader::LoadPrimitive(PrimitiveMeshType::Cube);

		for (Mesh::Vertex& vertex : m_CubePrimitive->m_Vertices)
		{
			vertex.Position *= 2.0f;
			vertex.Color = glm::vec3(1.0f);
		}
		m_CubePrimitive->Init();
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

		auto group = registry.group<CameraComponent>(entt::get<TransformComponent>);

		m_Shader->SetUniformBool("BIsDrawingFrustum", true);
		glm::vec4 frustumColor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
		m_Shader->SetUniformVec4("BaseColor", frustumColor);


		for (auto entity : group)
		{
			CameraComponent& camera = group.get<CameraComponent>(entity);
			if (camera.IsActive)
				continue;
			glm::mat4 viewMatrix = glm::inverse(group.get<TransformComponent>(entity).GetModelMatrix());
			//m_Shader->SetUniformMat4("Transform", transformMatrix);
			glm::mat4 frustMatrix = glm::inverse(camera.Cam.ProjectionMatrix() * viewMatrix);
			m_Shader->SetUniformMat4("Transform", frustMatrix);

			std::vector<glm::vec4> tempVerts;
			for (Mesh::Vertex& vertex : m_CubePrimitive->m_Vertices)
			{
				glm::vec4 newVert = glm::vec4(vertex.Position, 1.0f);
				newVert = frustMatrix * newVert;
				newVert = newVert / newVert.w;
				tempVerts.push_back(newVert);
			}
			m_CubePrimitive->Render(*m_Shader);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_Shader->EndUse();
	}
	
}