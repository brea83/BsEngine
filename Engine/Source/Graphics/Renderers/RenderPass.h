#pragma once
#include "Scene/Scene.h"

// ToDo: PBR lighting
// ToDo: Dithered lighting
// ToDo: Shadow pass
// ToDo: Lightmaps and shadow maps
// ToStudy: Deffered Rendering

namespace Pixie
{
	class FrameBuffer;
	class Shader;

	class RenderPass
	{
	public:
		virtual ~RenderPass() {  }
		virtual void Execute(Scene& sceneToRender, uint32_t prevPassDepthID = 0, uint32_t prevPassColorID = 0) = 0;
		virtual std::shared_ptr<Shader> GetShader() = 0;
		virtual std::shared_ptr<FrameBuffer> GetFrameBuffer() const = 0;
		virtual uint32_t GetFrameBufferID() = 0;
		virtual uint32_t GetColorAttatchmentID() = 0;
		virtual uint32_t GetDepthAttatchmentID() = 0;
	};

}