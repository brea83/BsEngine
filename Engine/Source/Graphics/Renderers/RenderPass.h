#pragma once
#include "Scene/Scene.h"

namespace Pixie
{
	class RenderPass
	{
	public:
		virtual void Execute(Scene& sceneToRender) = 0;
	};

}