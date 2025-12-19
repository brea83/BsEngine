#include "EditorLayer.h"
#include "BsPrecompileHeader.h"
#include "Pixie.h"
#include "Scene/SceneSerializer.h"

#include "ImGui/ImGuiPanel.h"
#include <imgui_internal.h>

#include <ImGuizmo/ImGuizmo.h>

#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "GLFW/glfw3.h"

#include "Panels/AssetViewerPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/DetailsViewPanel.h"

namespace Pixie
{
	EditorLayer::EditorLayer() : ImGuiLayer()
	{}

	EditorLayer::~EditorLayer()
	{
		delete m_AssetViewer;
	}

	void EditorLayer::OnAttach()
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

		m_CurrentScene = EngineContext::GetEngine()->GetScene();
		//m_Hierarchy.SetContext(EngineContext::GetEngine()->GetScene());

		//m_AssetViewer = new AssetViewerPanel();
	}

	void EditorLayer::OnDetach()
	{}



	void EditorLayer::OnImGuiRender()
	{
		EngineContext& engine = *EngineContext::GetEngine();
		static bool show = true;

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport()/*, ImGuiDockNodeFlags_PassthruCentralNode*/);
		DrawEditorMenu(&engine);
		ImGui::Begin("Window stats");
		ImGui::SeparatorText("FPS");
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("%.3f", io.Framerate);
		ImGui::SeparatorText("from Window Class");
		std::shared_ptr<Window> window = engine.GetWindow();
		ImGui::Text("Width: %d", window->WindowWidth());
		ImGui::Text("Height: %d", window->WindowHeight());
		ImGui::Text("AspectRatio: %f", ((float)window->WindowWidth() / (float)window->WindowHeight()));

		ImGui::End();

		// this function will change which entity m_Selected represents
		SceneHierarchyPanel::Draw(m_CurrentScene, m_Selected);

		//Details view can't change which entity m_Selected represents, but its components can be changed
		DetailsViewPanel::Draw(m_CurrentScene, m_Selected);
		AssetViewerPanel::Draw();

		DrawViewport(engine, m_Selected);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::DrawViewport(EngineContext& engine, GameObject& selected)
	{
		ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_MenuBar);
		DrawSceneTools();
		glm::mat4 viewMatrix{ 1.0f };
		Camera* camera = m_CurrentScene->GetActiveCamera(viewMatrix);
		//TransformComponent& camTransform = m_CurrentScene->GetActiveCameraTransform();
		//DrawGridLines(camera);

		std::shared_ptr<FrameBuffer> frameBuffer = engine.GetRenderer()->GetFrameBuffer();
		uint32_t textureID = frameBuffer->GetColorAttachmentRendererId();
		ImVec2 currentSize = ImGui::GetContentRegionAvail();

		if (m_ViewportPanelSize.x != currentSize.x || m_ViewportPanelSize.y != currentSize.y)
		{
			m_ViewportPanelSize = glm::vec2(currentSize.x, currentSize.y);
			frameBuffer->Resize(currentSize.x, currentSize.y);
			camera->SetAspectRatio((float)currentSize.x / (float)currentSize.y);


		}
		ImGui::Image((void*)textureID, currentSize, { 0, 1 }, { 1, 0 });
		DrawGizmos(camera, viewMatrix, selected);

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

	void EditorLayer::DrawEditorMenu(EngineContext* engine)
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) engine->StopApplication();

				if (ImGui::MenuItem("Test Save Scene"))
				{
					SceneSerializer serializer(m_CurrentScene);
					serializer.Serialize("TestSave.bin");
				}

				if (ImGui::MenuItem("Test Load Scene"))
				{
					SceneSerializer serializer(m_CurrentScene);
					serializer.Deserialize("TestSave.bin");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Change Scene Name test")) m_CurrentScene->SetName("NameFromMenu");

				if (ImGui::MenuItem("Empty GameObject"))
				{
					m_CurrentScene->CreateEmptyGameObject("Empty");
				}

				if (ImGui::MenuItem("Create Cube")) m_CurrentScene->CreateCube();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Show ImGui Metrics")) ImGui::ShowMetricsWindow();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

	}


	void EditorLayer::DrawSceneTools()
	{
		ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		float height = ImGui::GetFrameHeight();
		if (ImGui::BeginViewportSideBar("##SceneTools", viewport, ImGuiDir_Up, height, windowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::Button("CamFlyMode"))
				{
					EngineContext* engine = EngineContext::GetEngine();
					engine->ToggleCamFlyMode();
				}

				GameObject activeCam = m_CurrentScene->GetActiveCameraGameObject();
				CameraController& camController = activeCam.GetComponent<CameraController>();
				Camera& camera = activeCam.GetComponent<CameraComponent>().Cam;
				static float translationSpeed = camController.GetTranslationSpeed();
				static float rotationSpeed = camController.GetRotationSpeed();
				static float fov = camera.GetFov();

				ImGui::SetItemTooltip("Tab to toggle fly controlls");

				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.1f);
				ImGui::Text("Cam Speed");
				if (ImGui::DragFloat("##Speed", &translationSpeed))
				{
					camController.SetTranslationSpeed(translationSpeed);
				}
				ImGui::Text("Look Sensitivity");
				if (ImGui::DragFloat("##Sensitivity", &rotationSpeed))
				{
					camController.SetRotationSpeed(rotationSpeed);
				}
				ImGui::Text("FOV");
				if (ImGui::DragFloat("##FoVvalue", &fov))
				{
					camera.SetFov(fov);
				}
				if (ImGui::Button("ResetFoV"))
				{
					camera.SetFov(45.0f);
				}
				ImGui::PopItemWidth();
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

	}

	void EditorLayer::DrawGridLines(Camera* camera)
	{}

	void EditorLayer::DrawGizmos(Camera* camera, glm::mat4 viewMatrix, GameObject& selected)
	{
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		entt::registry& registry = m_CurrentScene->GetRegistry();
		if (!registry.valid(selected) || m_GizmoType == -1)
		{
			return;
		}

		ImGuizmo::SetDrawlist();

		//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
			m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

		TransformComponent& transform = selected.GetTransform();
		//if (transform == nullptr) return;
		glm::mat4 transformMatrix = transform.GetLocal();
		/*ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transform.GetPosition()),
			glm::value_ptr(transform.GetRotationEuler(AngleType::Degrees)),
			glm::value_ptr(transform.GetScale()),
			glm::value_ptr(transformMatrix));*/

			//glm::mat4 deltaMatrix{ 1.0f };


		float snapValues[3] = { 0.01f, 0.01f, 0.01f };
		ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(camera->ProjectionMatrix()),
			(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transformMatrix), nullptr, snapValues);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation;
			glm::vec3 rotation;
			glm::vec3 scale;
			TransformComponent::Decompose(transformMatrix, scale, rotation, translation);

			glm::vec3 oldRotation = transform.GetRotationEuler(AngleType::Radians);
			glm::vec3 deltaRotation = rotation - oldRotation;

			transform.SetPosition(translation);
			transform.SetScale(scale);
			transform.SetRotationEuler(oldRotation + deltaRotation, AngleType::Radians);

			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				//std::cout << "NEW ROTATION == " << transform.GetRotationEuler().x << ", " << transform.GetRotationEuler().y << ", " << transform.GetRotationEuler().z << std::endl;
			}
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.IsRepeat())
		{
			return false;
		}

		bool control = Input::IsKeyPressed(Inputs::Keyboard::LeftControl)
			|| Input::IsKeyPressed(Inputs::Keyboard::RightControl);

		bool shift = Input::IsKeyPressed(Inputs::Keyboard::LeftShift)
			|| Input::IsKeyPressed(Inputs::Keyboard::RightShift);

		using Key = Inputs::Keyboard;

		if (!ImGuizmo::IsUsing())
		{
			switch ((Inputs::Keyboard)event.GetKeyCode())
			{
			case Key::G:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::S:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			default:
				break;
			}
		}

		return false;
	}
}