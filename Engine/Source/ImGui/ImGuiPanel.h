#pragma once
#include <imgui/imgui.h>
#include<imgui/backends/imgui_impl_glfw.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

class ImGuiPanel
{
public :
	virtual ~ImGuiPanel(){}
	virtual bool Draw() = 0;
	virtual int DrawReturnsInt() = 0;
};