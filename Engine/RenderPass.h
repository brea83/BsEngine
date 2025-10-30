#pragma once
#include "Scene.h"

class RenderPass
{
public:
	virtual void Execute(Scene& sceneToRender) = 0;
};

