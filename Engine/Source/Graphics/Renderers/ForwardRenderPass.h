#pragma once
#include "RenderPass.h"
#include <glm/glm.hpp>
//#include <memory>

class Shader;
class Texture;

class ForwardRenderPass : public RenderPass
{
public:
	ForwardRenderPass();
	~ForwardRenderPass();
	// Inherited via RenderPass
	void Execute(Scene& sceneToRender) override;
private:
	Shader* _shader;
	Texture* _fallbackTexture;

};

