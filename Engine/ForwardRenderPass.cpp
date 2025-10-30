#include "ForwardRenderPass.h"
#include <vector>
#include "Shaders/Shader.h"
#include "Renderable.h"

ForwardRenderPass::ForwardRenderPass()
{
	_shader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
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
	for (Renderable* object : objectsToRender)
	{
		object->Render(*_shader);
	}
	_shader->EndUse();
}
