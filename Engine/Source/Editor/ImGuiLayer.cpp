#include "BsPrecompileHeader.h"
#include "ImGuiLayer.h"

#include "EngineContext.h"
#include "GlfwWrapper.h"
//#include "glad/glad.h"

#include <imgui.h>
#include<backends/imgui_impl_glfw.h>
#include<backends/imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"
#include "Graphics/Camera.h"

#include "Editor/Panels/AssetViewerPanel.h"
#include "Editor/Panels/DetailsViewPanel.h"


ImGuiLayer::ImGuiLayer()
{}

ImGuiLayer::~ImGuiLayer()
{
	delete _assetViewer;
}

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

	_currentScene = EngineContext::GetEngine()->GetScene();
	//_hierarchy.SetContext(EngineContext::GetEngine()->GetScene());

	//_assetViewer = new AssetViewerPanel();
}

void ImGuiLayer::OnDetach()
{}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::OnImGuiRender()
{
	EngineContext& engine = *EngineContext::GetEngine();
	static bool show = true;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	
	DrawEditorMenu(&engine);
	ImGui::Begin("Window stats");
	ImGui::SeparatorText("from Window Class");
	Window& window = engine.GetWindow();
	ImGui::Text("Width: %d", window.WindowWidth());
	ImGui::Text("Height: %d", window.WindowHeight());
	ImGui::Text("AspectRatio: %f", ((float) window.WindowWidth() / (float)window.WindowHeight()));

	ImGui::SeparatorText("From Scene Camera");
	ImGui::Text("AspectRatio: %f", engine.GetScene()->GetMainCamera()->GetAspectRatio());
	ImGui::End();

	/*ImGui::Begin("Viewport");
	uint32_t textureID = engine.GetRenderer()->GetFrameBufferID();
	ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });
	ImGui::End();*/

	int selected = SceneHierarchyPanel::Draw(_currentScene);

	DetailsViewPanel::Draw(_currentScene, selected);
	AssetViewerPanel::Draw();
}

void ImGuiLayer::DrawEditorMenu(EngineContext* engine)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) engine->StopApplication();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::MenuItem("Create Cube")) engine->GetScene()->CreateCube();

			if (ImGui::MenuItem("Create Model"))
			{
				//	TODO: figure out pop up window from menu
				
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	EngineContext& engine = *EngineContext::GetEngine();
	io.DisplaySize = ImVec2(engine.GetWindow().WindowWidth(), engine.GetWindow().WindowHeight());

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
