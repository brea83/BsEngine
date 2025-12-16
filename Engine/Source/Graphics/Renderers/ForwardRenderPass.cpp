#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Components/Transform.h"
#include "Scene/Components/CameraComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.h"
#include "Resources/AssetLoader.h"
#include "EngineContext.h"
#include "Scene/GameObject.h"


namespace Pixie
{
	ForwardRenderPass::ForwardRenderPass()
	{
		m_FallbackTexture = AssetLoader::LoadTexture("../Assets/Textures/teal.png");//new Texture("Assets/Textures/ffxivSnowman1.png");
		m_FallbackSpecularMap = AssetLoader::LoadTexture("../Assets/Textures/noise.png");

		m_Shader = AssetLoader::LoadShader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
	}

	ForwardRenderPass::~ForwardRenderPass()
	{ }

	void ForwardRenderPass::Execute(Scene& sceneToRender)
	{

		m_Shader->Use();

		m_Shader->SetUniformInt("Texture1", 0);
		m_Shader->SetUniformInt("SpecularMap", 1);
		m_FallbackSpecularMap->Bind(1);

		GameObject cameraEntity = sceneToRender.GetActiveCameraGameObject();
		TransformComponent& transform = cameraEntity.GetComponent<TransformComponent>();
		glm::vec4 camPosition = glm::vec4(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z, 1.0f);
		static glm::vec4 lastPosition = camPosition;

		glm::mat4 viewMatrix{1.0f};
		Camera* mainCam = sceneToRender.GetActiveCamera(viewMatrix);

		if (mainCam == nullptr)
		{
			std::cout << "No Camera in the scene is set to active" << std::endl;
			return;
		}

		m_Shader->SetUniformMat4("view", viewMatrix);
		m_Shader->SetUniformMat4("projection", mainCam->ProjectionMatrix());
		m_Shader->SetUniformVec4("cameraPosition", camPosition);
		if (lastPosition != camPosition)
		{
			std::cout << "CamPosition: " << camPosition.x << ", " << camPosition.y << ", " << camPosition.z << std::endl;
			lastPosition = camPosition;
		}

		entt::registry& registry = sceneToRender.GetRegistry();

		auto group = registry.group<MeshComponent>(entt::get<TransformComponent>);

		for (auto entity : group)
		{
			TransformComponent& transform = group.get<TransformComponent>(entity);
			MeshComponent& mesh = group.get<MeshComponent>(entity);

			if (!mesh.HasTexture())
			{
				m_FallbackTexture->Bind(0);
			}

			mesh.Render(*m_Shader, transform);
		}

		m_Shader->EndUse();
	}
}