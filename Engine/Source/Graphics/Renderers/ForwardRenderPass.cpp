#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Primitives/Renderable.h"
#include "Scene.h"
#include "Graphics/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.h"

ForwardRenderPass::ForwardRenderPass()
{
	_fallbackTexture = new Texture("Assets/Textures/ffxivSnowman1.png");

	_shader = new Shader("Source/Graphics/Shaders/VertexShader.glsl", "Source/Graphics/Shaders/FragmentShader.glsl");
	// TODO: determine right place to set up view and projection matrices
	_shader->Use();

	_viewMatrix = glm::mat4(1.0f);
	_projectionMatrix = glm::mat4(1.0f);

	glm::mat4 identityMatrix = glm::mat4(1.0f);
	_shader->SetUniformMat4("model", identityMatrix);

	_shader->EndUse();
}

ForwardRenderPass::~ForwardRenderPass()
{
	if (_shader)
	{
		delete _shader;
		_shader = nullptr;
	}
}

void ForwardRenderPass::Execute(Scene& sceneToRender)
{
	std::vector<Renderable*>& objectsToRender = sceneToRender.GetRenderables();

	_shader->Use();

	_fallbackTexture->Bind(0);
	_shader->SetUniformInt("Texture1", 0);

	Camera* mainCam = sceneToRender.GetMainCamera();
	_viewMatrix = mainCam->ViewMatrix();
	_projectionMatrix = mainCam->ProjectionMatrix();
	_shader->SetUniformMat4("view", _viewMatrix);
	_shader->SetUniformMat4("projection", _projectionMatrix);

	for (Renderable* object : objectsToRender)
	{
		object->Render(*_shader);

		// make sure this obj model transform is not applied to the next obj
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		_shader->SetUniformMat4("model", identityMatrix);
	}
	_shader->EndUse();
}
