#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Components/CameraComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.h"
#include "Resources/AssetLoader.h"
#include "EngineContext.h"
#include "Scene/Entity.h"


namespace Pixie
{
	ForwardRenderPass::ForwardRenderPass()
	{
		m_FallbackTexture = AssetLoader::LoadTexture("Assets/Textures/ffxivSnowman1.png");//new Texture("Assets/Textures/ffxivSnowman1.png");

		m_Shader = AssetLoader::LoadShader("Source/Graphics/Shaders/VertexShader.glsl", "Source/Graphics/Shaders/FragmentShader.glsl");
	}

	ForwardRenderPass::~ForwardRenderPass()
	{ }

	void ForwardRenderPass::Execute(Scene& sceneToRender)
	{

		m_Shader->Use();

		m_Shader->SetUniformInt("Texture1", 0);

		glm::mat4 viewMatrix{1.0f};
		Camera* mainCam = sceneToRender.GetActiveCamera(viewMatrix);

		if (mainCam == nullptr)
		{
			std::cout << "No Camera in the scene is set to active" << std::endl;
			return;
		}

		m_Shader->SetUniformMat4("view", viewMatrix);
		m_Shader->SetUniformMat4("projection", mainCam->ProjectionMatrix());
		

		entt::registry& registry = sceneToRender.GetRegistry();

		auto group = registry.group<MeshComponent>(entt::get<Transform>);

		for (auto entity : group)
		{
			Transform& transform = group.get<Transform>(entity);
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