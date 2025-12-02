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
	_fallbackTexture = AssetLoader::LoadTexture("Assets/Textures/ffxivSnowman1.png");//new Texture("Assets/Textures/ffxivSnowman1.png");

	_shader = AssetLoader::LoadShader("Source/Graphics/Shaders/VertexShader.glsl", "Source/Graphics/Shaders/FragmentShader.glsl");
}

ForwardRenderPass::~ForwardRenderPass()
{ }

void ForwardRenderPass::Execute(Scene& sceneToRender)
{
	std::vector<std::shared_ptr<MeshComponent>>& objectsToRender = sceneToRender.GetRenderables();

	_shader->Use();

	_shader->SetUniformInt("Texture1", 0);

	std::shared_ptr<Camera> mainCam = sceneToRender.GetActiveCamera();
	_shader->SetUniformMat4("view", (mainCam->ViewMatrix()));
	_shader->SetUniformMat4("projection", mainCam->ProjectionMatrix());

	entt::registry& registry = EngineContext::GetEngine()->GetRegistry();

	auto group = registry.group<Transform>(entt::get<MeshComponent>);
	for (auto entity : group)
	{
		Transform& transform = group.get<Transform>(entity);
		MeshComponent& mesh = group.get<MeshComponent>(entity);

		_fallbackTexture->Bind(0);
		mesh.Render(*_shader, transform);
	}
	
	for (auto object : objectsToRender)
	{
		_fallbackTexture->Bind(0);
		object->Render(*_shader);
	}

	_shader->EndUse();
}
