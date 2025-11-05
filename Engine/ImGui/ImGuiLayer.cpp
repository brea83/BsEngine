#include "ImGuiLayer.h"

#include "../EngineContext.h"
#include "../GlfwWrapper.h"
//#include "glad/glad.h"

#include <imgui.h>
#include<backends/imgui_impl_glfw.h>
#include<backends/imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"

ImGuiLayer::ImGuiLayer()
{}

ImGuiLayer::~ImGuiLayer()
{}

void ImGuiLayer::OnAttach()
{
	// setup dear imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
	//style set up
	ImGui::StyleColorsDark();

	// when viewports enabled tweak the window rounding/windowBg so platform windows can look identical
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	EngineContext* engine = EngineContext::GetEngine();

	// set up platfor/fenderer bindings
	ImGui_ImplGlfw_InitForOpenGL(engine->GetMainWindow()->GetGlfwWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiLayer::OnDetach()
{}

void ImGuiLayer::OnImGuiRender()
{
	static bool show = true;
	//ImGui::ShowDemoWindow(&show);
}

void ImGuiLayer::Begin()
{
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();
	//ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	//ImGuiIO& io = ImGui::GetIO();
	//Application& app = Application::Get();
	//io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

	// rendering
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//	glfwMakeContextCurrent(backup_current_context);
	//}
}
