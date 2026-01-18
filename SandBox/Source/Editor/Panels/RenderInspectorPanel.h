#pragma once
#include "ImGui/ImGuiPanel.h"
#include <memory>
namespace Pixie
{
	class FrameBuffer;
	class RenderInspectorPanel : public ImGuiPanel
	{
	public:
		RenderInspectorPanel() = default;
		// Inherited via ImGuiPanel
		bool Draw() override;
		int DrawReturnsInt() override { return 0; }

		struct DebugImageSpecification
		{
			ImVec2 ImageSize;
			ImVec2 Padding;
		};
	protected:

		void DrawFrameBuffer(std::shared_ptr<FrameBuffer> frameBuffer, const std::string& shaderName, float heightRestriction);
		DebugImageSpecification GetImageSize(ImVec2 regionAvailable, int numTextures, float bufferAspectRatio);
		
	};

}