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
#include <ImGuizmo/ImGuizmo.h>
#include <glm/matrix.hpp>


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
	//ToDo hook this up to detect perspective changes
	//ImGuizmo::BeginFrame();
}

void ImGuiLayer::OnImGuiRender()
{
	EngineContext& engine = *EngineContext::GetEngine();
	static bool show = true;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	//DrawGridLines();
	DrawEditorMenu(&engine);
	ImGui::Begin("Window stats");
	ImGui::SeparatorText("FPS");
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("%.3f", io.Framerate);
	ImGui::SeparatorText("from Window Class");
	Window& window = engine.GetWindow();
	ImGui::Text("Width: %d", window.WindowWidth());
	ImGui::Text("Height: %d", window.WindowHeight());
	ImGui::Text("AspectRatio: %f", ((float) window.WindowWidth() / (float)window.WindowHeight()));

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

	DrawSceneTools();
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

void ImGuiLayer::DrawSceneTools()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::Separator();
		if(ImGui::Button("CamFlyMode"))
		{
			EngineContext* engine = EngineContext::GetEngine();
			engine->ToggleCamFlyMode();
		}
		ImGui::SetItemTooltip("Tab to toggle fly controlls");

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.1f);
		ImGui::Text("Cam Speed");
		ImGui::DragFloat("##Speed", &_currentScene->GetMainCamera()->_cameraSpeed);
		ImGui::Text("Look Sensitivity");
		ImGui::DragFloat("##Sensitivity", &_currentScene->GetMainCamera()->_mouseLookSesitivity);
		ImGui::Text("FOV");
		ImGui::DragFloat("##FoVvalue", &_currentScene->GetMainCamera()->_fov);
		if(ImGui::Button("ResetFoV"))
		{
			_currentScene->GetMainCamera()->_fov = 45.0f;
		}
		ImGui::PopItemWidth();
		ImGui::EndMainMenuBar();
	}
	
}

void ImGuiLayer::DrawGridLines()
{
	Camera* mainCam = _currentScene->GetMainCamera();

	glm::mat4 view = mainCam->ViewMatrix();
	glm::mat4 projection = mainCam->ProjectionMatrix();
	float guizmoProjection[16] = 
	{
		projection[0][0], projection[1][0], projection[2][0], projection[3][0],
		projection[0][1], projection[1][1], projection[2][1], projection[3][1],
		projection[0][2], projection[1][2], projection[2][2], projection[3][2],
		projection[0][3], projection[1][3], projection[2][3], projection[3][3],
	};
	float guizmoView[16] = 
	{
		view[0][0], view[1][0], view[2][0], view[3][0],
		view[0][1], view[1][1], view[2][1], view[3][1],
		view[0][2], view[1][2], view[2][2], view[3][2],
		view[0][3], view[1][3], view[2][3], view[3][3],
	};

	static const float guizmoIdentity[16] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
	};

	//ImGuizmo::DrawGrid(guizmoView, guizmoProjection, guizmoIdentity, 100.0f);
}
