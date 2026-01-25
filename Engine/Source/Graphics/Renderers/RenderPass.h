#pragma once
#include "Scene/Scene.h"

// ToDo: PBR lighting
// ToDo: Dithered lighting
// ToDo: baked Lightmaps and shadow maps
// ToStudy: Deffered Rendering

namespace Pixie
{
	class FrameBuffer;
	class ModularFrameBuffer;
	class Shader;

	class RenderPass
	{
	public:
		virtual ~RenderPass() {  }
		virtual void Execute(Scene& sceneToRender, uint32_t prevPassDepthID = 0, uint32_t prevPassColorID = 0) = 0;
		virtual std::shared_ptr<Shader> GetShader() = 0;
		virtual std::shared_ptr<ModularFrameBuffer> GetFrameBuffer() const = 0;
		virtual uint32_t GetFrameBufferID() const = 0;
		virtual uint32_t GetColorAttatchmentID() const = 0;
		virtual uint32_t GetDepthAttatchmentID() const = 0;
		virtual bool IsLit() const = 0;
		virtual void ForceLightsOff(bool value) = 0;
	};

}