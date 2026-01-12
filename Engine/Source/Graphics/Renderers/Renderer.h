#pragma once
#include <vector>
#include <memory>
#include "Scene/Scene.h"
#include "RenderPass.h"
#include "Graphics/FrameBuffer.h"

namespace Pixie
{
	class EngineContext;
	class Shader;
	class Mesh;

	class Renderer
	{
	public:
		virtual void Init() = 0;
		virtual void BeginFrame(Scene& scene) = 0;
		virtual void RenderFrame(Scene& scene) = 0;
		virtual void EndFrame(Scene& scene) = 0;

		std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return m_FrameBuffer; }
		virtual uint32_t GetFrameBufferID() { return m_FrameBuffer->GetColorAttachmentID(); }

		std::shared_ptr<Shader> GetGridShader() { return m_GridShader; }
		void EnableGridShader(bool value) { m_DrawGridEnabled = value; }
		bool IsGridShaderEnabled() { return m_DrawGridEnabled; }
	protected:
		EngineContext* m_Engine;
		std::vector<std::unique_ptr<RenderPass>> m_Passes;

		std::shared_ptr<FrameBuffer> m_FrameBuffer;

		bool m_DrawGridEnabled;
		std::shared_ptr<Shader> m_GridShader;
		std::shared_ptr<Mesh> m_EditorGrid;

	};

}