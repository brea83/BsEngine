#include "BsPrecompileHeader.h"
#include "ImGuiLayer.h"

#include "EngineContext.h"
#include "GlfwWrapper.h"
#include "Graphics/Camera.h"
#include "Graphics/FrameBuffer.h"
#include "Scene/GameObject.h"
#include "Graphics/Primitives/Transform.h"
//#include "glad/glad.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <ImGuizmo/ImGuizmo.h>

#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLFW/glfw3.h"

#include "Editor/Panels/AssetViewerPanel.h"
#include "Editor/Panels/DetailsViewPanel.h"


ImGuiLayer::ImGuiLayer()
{}

ImGuiLayer::~ImGuiLayer()
{
	delete m_AssetViewer;
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

	Pixie::EngineContext* engine = Pixie::EngineContext::GetEngine();

	// set up imgui connection to glfw and open gl
	ImGui_ImplGlfw_InitForOpenGL(engine->GetGlfwWindow(), true);
	ImGui_ImplOpenGL3_Init();

	m_CurrentScene = Pixie::EngineContext::GetEngine()->GetScene();
	//m_Hierarchy.SetContext(EngineContext::GetEngine()->GetScene());

	//m_AssetViewer = new AssetViewerPanel();
}

void ImGuiLayer::OnDetach()
{}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ToDo hook this up to detect perspective changes
	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(false);
}

void ImGuiLayer::OnImGuiRender()
{
	Pixie::EngineContext& engine = *Pixie::EngineContext::GetEngine();
	static bool show = true;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport()/*, ImGuiDockNodeFlags_PassthruCentralNode*/);
	DrawEditorMenu(&engine);
	ImGui::Begin("Window stats");
	ImGui::SeparatorText("FPS");
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("%.3f", io.Framerate);
	ImGui::SeparatorText("from Window Class");
	Pixie::Window& window = engine.GetWindow();
	ImGui::Text("Width: %d", window.WindowWidth());
	ImGui::Text("Height: %d", window.WindowHeight());
	ImGui::Text("AspectRatio: %f", ((float) window.WindowWidth() / (float)window.WindowHeight()));

	ImGui::End();

	int selected = SceneHierarchyPanel::Draw(m_CurrentScene);

	DetailsViewPanel::Draw(m_CurrentScene, selected);
	AssetViewerPanel::Draw();

	DrawViewport(engine, selected);
}

void ImGuiLayer::DrawViewport(Pixie::EngineContext& engine, int selected)
{
	ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_MenuBar);
	DrawSceneTools();
	std::shared_ptr<Pixie::Camera> camera = m_CurrentScene->GetActiveCamera();
	//DrawGridLines(camera);

	std::shared_ptr<Pixie::FrameBuffer> frameBuffer = engine.GetRenderer()->GetFrameBuffer();
	uint32_t textureID = frameBuffer->GetColorAttachmentRendererId();
	ImVec2 currentSize = ImGui::GetContentRegionAvail();

	if (m_ViewportPanelSize.x != currentSize.x || m_ViewportPanelSize.y != currentSize.y)
	{
		m_ViewportPanelSize = glm::vec2(currentSize.x, currentSize.y);
		frameBuffer->Resize(currentSize.x, currentSize.y);
		camera->SetAspectRatio((float)currentSize.x / (float)currentSize.y);
	}
	ImGui::Image((void*)textureID, currentSize, { 0, 1 }, { 1, 0 });
	DrawGizmos(camera, selected);

	if (ImGui::IsWindowHovered())
	{
		ImGuiIO& io = ImGui::GetIO();

		if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
		{

		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{

		}
	}
	ImGui::End();
}

void ImGuiLayer::DrawEditorMenu(Pixie::EngineContext* engine)
{
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) engine->StopApplication();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Empty GameObject"))
			{
				Pixie::GameObject* testObject = new Pixie::GameObject();
				m_CurrentScene->AddGameObject(testObject);
			}

			if (ImGui::MenuItem("Create Cube")) m_CurrentScene->CreateCube();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Pixie::EngineContext& engine = *Pixie::EngineContext::GetEngine();
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
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();
	if (ImGui::BeginViewportSideBar("##SceneTools", viewport, ImGuiDir_Up, height, windowFlags))
	{
		if (ImGui::BeginMenuBar())
		{
			if(ImGui::Button("CamFlyMode"))
			{
				Pixie::EngineContext* engine = Pixie::EngineContext::GetEngine();
				engine->ToggleCamFlyMode();
			}
			ImGui::SetItemTooltip("Tab to toggle fly controlls");

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.1f);
			ImGui::Text("Cam Speed");
			ImGui::DragFloat("##Speed", &m_CurrentScene->GetActiveCamera()->m_CameraSpeed);
			ImGui::Text("Look Sensitivity");
			ImGui::DragFloat("##Sensitivity", &m_CurrentScene->GetActiveCamera()->m_MouseLookSesitivity);
			ImGui::Text("FOV");
			ImGui::DragFloat("##FoVvalue", &m_CurrentScene->GetActiveCamera()->m_Fov);
			if(ImGui::Button("ResetFoV"))
			{
				m_CurrentScene->GetActiveCamera()->m_Fov = 45.0f;
			}
			ImGui::PopItemWidth();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
	
}

void ImGuiLayer::DrawGridLines(std::shared_ptr<Pixie::Camera> camera)
{
}

void ImGuiLayer::DrawGizmos(std::shared_ptr<Pixie::Camera> camera, int selectedObjectIndex)
{
	Pixie::GameObject* selectedObject = m_CurrentScene->GetGameObjectByIndex(selectedObjectIndex);
	if (selectedObject == nullptr) return;

	ImGuizmo::SetDrawlist();

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
	
	std::shared_ptr<Pixie::Transform> transform = selectedObject->GetTransform();
	glm::mat4 transformMatrix = transform->GetLocal();

	ImGuizmo::Manipulate(glm::value_ptr(camera->ViewMatrix()), glm::value_ptr(camera->ProjectionMatrix()),
		ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transformMatrix));
	
	if (ImGuizmo::IsUsing())
	{
		transform->SetPosition(glm::vec3(transformMatrix[3]));
	}
}
