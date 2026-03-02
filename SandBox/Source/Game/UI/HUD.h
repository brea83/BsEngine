#pragma once
#include "ImGui/ImGuiPanel.h"
#include <memory>

namespace Pixie
{
	class HUD : public ImGuiPanel
	{
	public:
		HUD() = default;
		HUD(const HUD&) = default;
		// Inherited via ImGuiPanel
		bool Draw() override;
		int DrawReturnsInt() override;

		ImVec2 work_pos{ 0.0f, 0.0f };
		ImVec2 work_size{ 10.0f, 10.0f };
	};
}

// 1030 am lecture tomorrow, might be with shramthrax?
/*

should get a partial or full list of companies coming to the match up by end of day

school will try and get companies that wanted to join but couldn't do this day to have connection days



*/