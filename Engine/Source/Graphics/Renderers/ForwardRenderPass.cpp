#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Scene.h"
#include "Graphics/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.h"
#include "Resources/AssetLoader.h"
#include "EngineContext.h"

ForwardRenderPass::ForwardRenderPass()
{
	m_FallbackTexture = AssetLoader::LoadTexture("Assets/Textures/ffxivSnowman1.png");//new Texture("Assets/Textures/ffxivSnowman1.png");

	m_Shader = AssetLoader::LoadShader("Source/Graphics/Shaders/VertexShader.glsl", "Source/Graphics/Shaders/FragmentShader.glsl");
}

ForwardRenderPass::~ForwardRenderPass()
{ }

void ForwardRenderPass::Execute(Scene& sceneToRender)
{
	std::vector<std::shared_ptr<MeshComponent>>& objectsToRender = sceneToRender.GetRenderables();

	m_Shader->Use();

	m_Shader->SetUniformInt("Texture1", 0);

	std::shared_ptr<Camera> mainCam = sceneToRender.GetActiveCamera();
	m_Shader->SetUniformMat4("view", (mainCam->ViewMatrix()));
	m_Shader->SetUniformMat4("projection", mainCam->ProjectionMatrix());

	entt::registry& registry = EngineContext::GetEngine()->GetRegistry();

	auto group = registry.group<Transform>(entt::get<MeshComponent>);
	for (auto entity : group)
	{
		Transform& transform = group.get<Transform>(entity);
		MeshComponent& mesh = group.get<MeshComponent>(entity);

		m_FallbackTexture->Bind(0);
		mesh.Render(*m_Shader, transform);
	}
	
	for (auto object : objectsToRender)
	{
		m_FallbackTexture->Bind(0);
		object->Render(*m_Shader);
	}

	m_Shader->EndUse();
}
