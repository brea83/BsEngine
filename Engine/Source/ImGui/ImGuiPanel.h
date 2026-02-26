#pragma once
#include <imgui/imgui.h>
#include<imgui/backends/imgui_impl_glfw.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <glm/glm.hpp>

namespace Pixie
{
	class ImGuiPanel
	{
	public:
		virtual ~ImGuiPanel() {}
		virtual bool Draw() = 0;
		virtual int DrawReturnsInt() = 0;

		struct SliderParams
		{
			float ResetValue{ 0.0f };
			float Speed{ 0.1f };
			float Min{ 0.0f };
			float Max{ 0.0f };
			std::string Format{ "%.3f" };
			int Flags{ 0 };
		};

		static bool DrawVec3Control(const std::string& label, glm::vec3& values, SliderParams params, float columnWidth = 5.0f);
		static bool DrawVec2Control(const std::string& label, glm::vec2& values, SliderParams params, float columnWidth = 5.0f);
		static bool DrawFloatControl(const std::string& label, float& value, SliderParams params, float columnWidth = 5.0f);

		static bool DrawStringProperty(const std::string& label, std::string& value, std::string& editingValue, float propertyWidth = 10.0f);

		static bool FileProperty(const std::string& label, std::string& value, const char* filter, std::string deleteButtonText = "X", float propertyWidth = 0.0f);

		static ImVec2 GetTextSizePadded(const std::string& label = "X");

		static void CenteredText(const std::string& text);
	};
}