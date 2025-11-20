#pragma once
#include <imgui.h>
#include<backends/imgui_impl_glfw.h>
#include<backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>

class ImGuiPanel
{
public :
	virtual ~ImGuiPanel(){}
	virtual bool Draw() = 0;
	virtual int DrawReturnsInt() = 0;
};