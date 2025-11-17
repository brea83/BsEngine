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
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Texture> _fallbackTexture;

};

