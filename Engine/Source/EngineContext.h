#pragma once
#include "Core.h"
#include <deque>
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include <glm/glm.hpp>

struct GLFWwindow;
namespace Pixie
{
	class Window;
	class Scene;
	class Renderer;
	class ImGuiLayer;

	class EngineContext
	{
	public:
		static EngineContext* GetEngine();
		virtual ~EngineContext() { }
		bool Init();

		static int NextUID;

		std::shared_ptr<Window> GetWindow() { return m_MainWindow; }
		GLFWwindow* GetGlfwWindow();
		glm::vec2 GetViewportSize() const;

		glm::vec2 GetWindowSize() const;

		void SetScene(Scene* newScene) { m_ActiveScene = newScene; }
		Scene* GetScene() { return m_ActiveScene; }

		Renderer* GetRenderer() { return m_Renderer; }

		void SetImGuiLayer(ImGuiLayer* layer) { m_ImGuiLayer = layer; }

		// methods
		bool IsRunning()const { return m_IsRunning; }
		void StopApplication() { m_IsRunning = false; }
		bool IsEditorEnabled() { return m_EditorEnabled; }
		// called in main loop before draw and update
		// itterates trhough event queue and sends them to appropriate systems
		void DispatchEvents();
		// called in main loop after Dispatch Events, before Update
		void Draw();
		// called in main loop after Events, and Draw
		void Update();
		void DrawConsole();

		void ToggleCamFlyMode();

		// callback sent to GLFW window system that collects events each frame 
		// to be processed at the top of next frame in DispatchEvents
		void OnEvent(Event& event);

		bool OnFrameBufferSize(WindowResizedEvent& event);
		bool OnWindowClosed(WindowClosedEvent& event);

	protected:
		// constructors, properties, getters and setters
		EngineContext(Window* startingWindow = nullptr, Scene* startingScene = nullptr, Renderer* startingRenderer = nullptr, ImGuiLayer* startingImGuiLayer = nullptr);

		static EngineContext* m_Engine;
		std::deque<std::shared_ptr<Event>> m_EventQueue;

		// properties
		bool m_IsRunning{ true };
		bool m_IsMinimized{ false };
		bool m_EditorEnabled{ true };

		bool m_CamFlyMode{ false };
		bool m_FirstMouse{ false };
		float m_PrevMouseX{ 0.0f };
		float m_PrevMouseY{ 0.0f };

		float m_DeltaTime{ 0.0f };
		float m_LastFrameTime{ 0.0f };

		std::shared_ptr<Window> m_MainWindow;
		Scene* m_ActiveScene;
		Renderer* m_Renderer;

		ImGuiLayer* m_ImGuiLayer{ nullptr };

		// DebugConsole* _console;
		// methods
		template <typename T>
		void EnqueEvent(T& event)
		{
			m_EventQueue.push_back(std::make_shared<T>(event));
		}
		void DispatchEvent(std::shared_ptr<Event> eventptr);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		/*{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}*/
	};
	// To be defined in Client (ie SandBox)
	EngineContext* CreateApplication();
}