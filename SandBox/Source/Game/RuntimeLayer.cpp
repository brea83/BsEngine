#include "RuntimeLayer.h"
#include "Pixie.h"
#include "ExampleGame.h"
#include "StateMachine/GameStates.h"

#include "ImGui/ImGuiPanel.h"
#include "ImGui/imgui_sink.h"
#include "ImGui/StartUpWindow.h"
#include "ImGui/ConsoleWindow.h"

namespace Pixie
{
	std::shared_ptr<Game> RuntimeLayer::GetGame()
	{
		return std::dynamic_pointer_cast<Game>(m_Game);
	}

	void Pixie::RuntimeLayer::OnAttach()
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
		//ImGui::StyleColorsDark();
		StyleColorsPixie();

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

		m_CurrentScene = engine->GetScene();;

		m_ConsoleWindow = std::make_shared<ConsoleWindow>();

		spdlog::sink_ptr imguiSink = std::make_shared< spdlog::imgui_sink<std::mutex>>(m_ConsoleWindow);
		imguiSink->set_pattern("%^%v%$");
		Logger::GetCoreLogger()->sinks().push_back(imguiSink);
		Logger::GetSandboxLogger()->sinks().push_back(imguiSink);

		m_ViewportPanelSize = engine->GetWindowSize();

		m_Game = std::make_shared<ExampleGame>();
		
	}

	void RuntimeLayer::OnEvent(Event& event)
	{
		if (m_Game)
			m_Game->OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<SceneChangedEvent>(BIND_EVENT_FUNCTION(RuntimeLayer::OnSceneChangedEvent));


	}
	bool RuntimeLayer::OnSceneChangedEvent(SceneChangedEvent& event)
	{
		std::shared_ptr<Scene> newScene = event.GetScene();

		if (m_CurrentScene == newScene)
			return false;

		m_CurrentScene = newScene;
		return false;
	}

	void RuntimeLayer::OnUpdate(float deltaTime)
	{
		if (m_Game == nullptr) return;

		m_Game->OnUpdate(deltaTime);
	}

	void RuntimeLayer::OnImGuiRender()
	{
		EngineContext& engine = *EngineContext::GetEngine();
		static bool show = true;

		if (!m_GameSelected)
		{
			m_GameSelected = StartUpWindow::DrawGameStartupWindow(std::dynamic_pointer_cast<Game>(m_Game));
			if (m_GameSelected)
			{
				m_Game->RequestLevelChange(0);
				m_Game->RequestStateChange(TitleState::Type());
			}
		}
	
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		DrawViewport(engine);



		m_Game->OnImGuiRender();
	}

	void RuntimeLayer::DrawViewport(EngineContext& engine)
	{
		ImGui::Begin("SplineGame", NULL, ImGuiWindowFlags_MenuBar);
		

		if (ImGui::BeginMainMenuBar())
		{
			std::string_view currentState = m_Game->GetCurrentState()->GetType();

			if (currentState == PlayingState::Type())
			{
				if (ImGui::Button("PAUSE"))
				{
					m_Game->RequestStateChange(PauseState::Type());
				}
			}

			ImGui::EndMainMenuBar();
		}

		glm::mat4 viewMatrix{ 1.0f };
		Camera* camera = m_CurrentScene->GetActiveCamera(viewMatrix);

		std::shared_ptr<FrameBuffer> frameBuffer = engine.GetRenderer()->GetFrameBuffer();
		uint32_t textureID = frameBuffer->GetColorAttachmentID();
		ImVec2 currentSize = ImGui::GetContentRegionAvail();

		if (m_ViewportPanelSize.x != currentSize.x || m_ViewportPanelSize.y != currentSize.y)
		{
			m_ViewportPanelSize = glm::vec2(currentSize.x, currentSize.y);
			frameBuffer->Resize((uint32_t)currentSize.x, (uint32_t)currentSize.y);

			m_CurrentScene->ForwardAspectRatio(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			//if (camera) camera->SetAspectRatio((float)currentSize.x / (float)currentSize.y);

		}
		ImGui::Image((void*)textureID, currentSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
	}

}