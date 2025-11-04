#include "ForwardRenderPass.h"
#include <vector>
#include "Shaders/Shader.h"
#include "Renderable.h"
#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

ForwardRenderPass::ForwardRenderPass()
{
	_shader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
	// TODO: determine right place to set up view and projection matrices
	_shader->Use();

	/*_viewMatrix = glm::mat4(1.0f);
	_viewMatrix = glm::translate(_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	_projectionMatrix = glm::mat4(1.0f);
	_projectionMatrix = glm::perspective(glm::radians(45.0f), (800.0f / 600.0f), 0.1f, 100.0f);
	*/
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
