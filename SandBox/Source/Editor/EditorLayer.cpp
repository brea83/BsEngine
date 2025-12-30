#include "EditorLayer.h"
#include "BsPrecompileHeader.h"
#include "Pixie.h"
#include "Scene/SceneSerializer.h"
#include "PlatformUtils.h"

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
		//delete m_AssetViewer;
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
		if (m_CurrentScene == nullptr)
		{
			ImGui::End();
			return;
		}

		glm::mat4 viewMatrix{ 1.0f };
		Camera* camera = m_CurrentScene->GetActiveCamera(viewMatrix);

		std::shared_ptr<FrameBuffer> frameBuffer = engine.GetRenderer()->GetFrameBuffer();
		uint32_t textureID = frameBuffer->GetColorAttachmentRendererId();
		ImVec2 currentSize = ImGui::GetContentRegionAvail();

		if (m_ViewportPanelSize.x != currentSize.x || m_ViewportPanelSize.y != currentSize.y)
		{
			m_ViewportPanelSize = glm::vec2(currentSize.x, currentSize.y);
			frameBuffer->Resize(currentSize.x, currentSize.y);
			
			if(camera) camera->SetAspectRatio((float)currentSize.x / (float)currentSize.y);


		}
		ImGui::Image((void*)textureID, currentSize, { 0, 1 }, { 1, 0 });
	
		if(camera)
			DrawGizmos(camera, viewMatrix, selected);

		ImGui::End();
	}

	void EditorLayer::DrawEditorMenu(EngineContext* engine)
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
				{
					OpenScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save Scene...", "Ctrl+S"))
				{
					SaveScene();
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) engine->StopApplication();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Change Scene Name test")) m_CurrentScene->SetName("NameFromMenu");

				if (ImGui::MenuItem("Populate With Test Objects"))
				{
					m_CurrentScene->PopulateWithTestObjects();
				}

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

	void EditorLayer::NewScene()
	{
		Scene* loadedScene = new Scene();
		EngineContext::GetEngine()->SetScene(loadedScene);
		OnSceneChange(loadedScene);
	}

	void EditorLayer::SaveScene()
	{
		if (!m_CurrentScenePath.empty())
		{
			SceneSerializer serializer(m_CurrentScene);
			serializer.Serialize(m_CurrentScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDialogs::SaveFile("Pixie Scene (*.pixie)\0*.pixie\0");

		if (!filePath.empty())
		{
			SceneSerializer serializer(m_CurrentScene);
			serializer.Serialize(filePath);
		}
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDialogs::OpenFile("Pixie Scene (*.pixie)\0*.pixie\0");

		if (!filePath.empty())
		{
			Scene* loadedScene = new Scene();
			EngineContext::GetEngine()->SetScene(loadedScene);
			SceneSerializer serializer(loadedScene);
			serializer.Deserialize(filePath);
			OnSceneChange(loadedScene, filePath);
		}
	}


	void EditorLayer::DrawSceneTools()
	{
		ImGuiViewportP* mainWindow = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		float height = ImGui::GetFrameHeight();
		if (ImGui::BeginViewportSideBar("##SceneTools", mainWindow, ImGuiDir_Up, height, windowFlags))
		{
			if (m_CurrentScene == nullptr)
			{
				ImGui::End;
				return;
			}
			if (ImGui::BeginMenuBar())
			{
				GameObject activeCam = m_CurrentScene->GetActiveCameraGameObject();
				if (activeCam)
				{
					DrawEditorCamTools(activeCam);
				}
				
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

	}

	void EditorLayer::DrawEditorCamTools(GameObject& activeCam)
	{

		CameraController& camController = activeCam.GetComponent<CameraController>();
		Camera& camera = activeCam.GetComponent<CameraComponent>().Cam;
		static float translationSpeed = camController.GetTranslationSpeed();
		static float rotationSpeed = camController.GetRotationSpeed();
		static float fov = camera.GetFov();

		ImGuiStyle& style = ImGui::GetStyle();
		const float global_scale = style.FontScaleMain * style.FontScaleDpi;

		ImGui::PushFont(nullptr, style.FontSizeBase * 1.2 * global_scale);
		ImGui::Text("Active Camera: ");
		ImGui::SameLine();
		ImGui::Text(activeCam.GetComponent<NameComponent>().Name.c_str());
		ImGui::PopFont();

		if (ImGui::Button("CamFlyMode"))
		{
			if (camController.GetMoveType() != CameraMoveType::Fly)
			{
				camController.SetMoveType(CameraMoveType::Fly);
			}
			else
			{
				camController.SetMoveType(CameraMoveType::END);
			}
		}

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
		glm::mat4 transformMatrix = transform.GetObjectToWorldMatrix();
		//glm::mat4 localTransform = transform.GetLocal();
		
		glm::vec3 oldTranslation;
		glm::vec3 oldRotation;
		glm::vec3 oldScale;
		TransformComponent::Decompose(transformMatrix, oldScale, oldRotation, oldTranslation);
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


			glm::vec3 localTranslation = transform.GetPosition();
			glm::vec3 localRotation = transform.GetRotationEuler(AngleType::Radians);
			glm::vec3 localScale = transform.GetScale();

			glm::vec3 deltaTranslation = translation - oldTranslation;
			glm::vec3 deltaRotation = rotation - oldRotation;
			glm::vec3 deltaScale = scale - oldScale;

			transform.SetPosition(localTranslation + deltaTranslation);
			transform.SetScale(localScale + deltaScale);
			transform.SetRotationEuler(localRotation + deltaRotation, AngleType::Radians);

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

		Key pressed = (Key)event.GetKeyCode();

		switch (pressed)
		{
		case Key::O:
			if (control)
			{
				OpenScene();
				return true;
			}
			break;
		case Key::N:
			if (control)
			{
				NewScene();
				return true;
			}
			break;
		case Key::S:
			if (control)
			{
				if (shift)
				{
					SaveSceneAs();
				}
				else
				{
					SaveScene();
				}
				return true;
			}
			break;
		default:
			break; 
		}

		ImGuiIO& io = ImGui::GetIO();
		if (!ImGuizmo::IsUsing() && !io.WantTextInput)
		{
			switch (pressed)
			{
			case Key::G:
				if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
				{
					m_GizmoType = -1;
				}
				else
				{
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				}
				break;
			case Key::R:
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				{
					m_GizmoType = -1;
				}
				else
				{
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				}
				break;
			case Key::S:
				if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
				{
					m_GizmoType = -1;
				}
				else
				{
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			default:
				break;
			}
		}

		return false;
	}
}