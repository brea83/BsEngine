#pragma once
#include <vector>
#include <memory>
#include "Scene.h"
#include "RenderPass.h"

class EngineContext;

class Renderer
{
public:
	virtual void Init() = 0;
	virtual void BeginFrame(Scene& scene) = 0;
	virtual void RenderFrame(Scene& scene) = 0;
	virtual void EndFrame(Scene& scene) = 0;
protected:
	EngineContext* _engine;
	std::vector<std::unique_ptr<RenderPass>> _passes;
};

