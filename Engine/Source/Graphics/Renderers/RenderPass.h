#pragma once
#include "Scene/Scene.h"

// ToDo: PBR lighting
// ToDo: Dithered lighting
// ToDo: Shadow pass
// ToDo: Lightmaps and shadow maps
// ToStudy: Deffered Rendering

namespace Pixie
{
	class RenderPass
	{
	public:
		virtual void Execute(Scene& sceneToRender) = 0;
	};

}