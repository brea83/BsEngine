#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Components/Component.h"
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

		GameObject mainLight = sceneToRender.GetMainLight();
		if (mainLight)
		{
			DirectionalLight& light = mainLight.GetComponent<DirectionalLight>();
			glm::vec3 direction = -1.0f * glm::normalize(light.Direction);
			m_Shader->SetUniformBool("bUseMainLight", true);
			m_Shader->SetUniformVec3("MainLight.direction", direction);
			m_Shader->SetUniformVec3("MainLight.color", light.Color);
			m_Shader->SetUniformVec3("MainLight.attenuation", light.Attenuations);
		}

		GameObject cameraEntity = sceneToRender.GetActiveCameraGameObject();
		TransformComponent& transform = cameraEntity.GetComponent<TransformComponent>();
		glm::vec3 camPosition = transform.GetPosition();


		glm::mat4 viewMatrix{1.0f};
		Camera* mainCam = sceneToRender.GetActiveCamera(viewMatrix);
		if (mainCam == nullptr)
		{
			std::cout << "No Camera in the scene is set to active" << std::endl;
			return;
		}
		glm::mat4 projectionMatrix = mainCam->ProjectionMatrix();

		m_Shader->SetUniformMat4("view", viewMatrix);
		m_Shader->SetUniformMat4("projection", projectionMatrix);
		m_Shader->SetUniformVec3("cameraPosition", camPosition);

		entt::registry& registry = sceneToRender.GetRegistry();

		auto group = registry.group<MeshComponent>(entt::get<TransformComponent>);

		for (auto entity : group)
		{
			TransformComponent& transform = group.get<TransformComponent>(entity);
			m_Shader->SetUniformMat4("transform", transform.GetObjectToWorldMatrix());

			MeshComponent& mesh = group.get<MeshComponent>(entity);

			if (!mesh.HasTexture())
			{
				m_FallbackTexture->Bind(0);
				m_Shader->SetUniformInt("Texture1", 0);
			}

			mesh.Render(*m_Shader);
		}

		m_Shader->EndUse();
	}
}