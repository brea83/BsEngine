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

	protected:
		void DrawFrameBuffer(std::shared_ptr<FrameBuffer> frameBuffer, const std::string& shaderName, float heightRestriction);
	};

}