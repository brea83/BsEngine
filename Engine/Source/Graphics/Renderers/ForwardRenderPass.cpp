#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Primitives/Renderable.h"
#include "Scene.h"
#include "Graphics/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.h"
#include "AssetLoader.h"

ForwardRenderPass::ForwardRenderPass()
{
	_fallbackTexture = AssetLoader::LoadTexture("Assets/Textures/ffxivSnowman1.png");//new Texture("Assets/Textures/ffxivSnowman1.png");

	_shader = AssetLoader::LoadShader("Source/Graphics/Shaders/VertexShader.glsl", "Source/Graphics/Shaders/FragmentShader.glsl");
}

ForwardRenderPass::~ForwardRenderPass()
{ }

void ForwardRenderPass::Execute(Scene& sceneToRender)
{
	std::vector<Renderable*>& objectsToRender = sceneToRender.GetRenderables();

	_shader->Use();

	_shader->SetUniformInt("Texture1", 0);

	Camera* mainCam = sceneToRender.GetMainCamera();
	_shader->SetUniformMat4("view", (mainCam->ViewMatrix()));
	_shader->SetUniformMat4("projection", mainCam->ProjectionMatrix());

	for (Renderable* object : objectsToRender)
	{
		_fallbackTexture->Bind(0);
		object->Render(*_shader);
	}
	_shader->EndUse();
}
