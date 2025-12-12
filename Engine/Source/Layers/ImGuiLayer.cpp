#include "BsPrecompileHeader.h"
#include "ImGuiLayer.h"
#include "EngineContext.h"
#include "Editor/Panels/ImGuiPanel.h"


#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"


namespace Pixie
{
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

		// set up imgui connection to glfw and open gl
		ImGui_ImplGlfw_InitForOpenGL(engine->GetGlfwWindow(), true);
		ImGui_ImplOpenGL3_Init();

	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		
		//rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
		
}