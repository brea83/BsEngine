#pragma once
#include "RenderPass.h"
#include <memory>

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
};

