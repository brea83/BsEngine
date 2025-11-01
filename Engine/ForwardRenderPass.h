#pragma once
#include "RenderPass.h"
#include <glm/glm.hpp>
//#include <memory>

class Shader;

class ForwardRenderPass : public RenderPass
{
public:
	ForwardRenderPass();
	~ForwardRenderPass();
	// Inherited via RenderPass
	void Execute(Scene& sceneToRender) override;
private:
	Shader* _shader;
	// TODO: PUT THESE SOMEWHERE SMARTER

	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;
};

