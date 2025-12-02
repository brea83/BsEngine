#pragma once
//#include <GLFW/glfw3.h>
#include "GlfwWrapper.h"
#include "Scene/Scene.h"
#include "Graphics/Renderers/ForwardRenderer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Editor/ImGuiLayer.h"
#include <EnTT/entt.hpp>


//class Renderer;
class Scene;
class Window;
struct GLFWwindow;
//class DebugConsole;

class EngineContext
{
public:
	// delete copy constructor
	EngineContext(const EngineContext& obj) = delete;

	static EngineContext* GetEngine();
	bool Init();
	
	static int NextUID;

	Window& GetWindow() { return *m_MainWindow; }
	GLFWwindow* GetGlfwWindow() { return m_MainWindow->GetGlfwWindow(); }

	void SetScene(Scene* newScene) { m_ActiveScene = newScene; }
	Scene* GetScene() { return m_ActiveScene; }

	Renderer* GetRenderer() { return m_Renderer; }

	entt::registry& GetRegistry() { return m_Registry; }

	// methods
	bool IsRunning()const { return m_IsRunning; }
	void StopApplication() { m_IsRunning = false; }
	void Update();
	void Draw();
	void DrawConsole();

	void ToggleCamFlyMode();

	// Window event callbacks
	void OnEvent(Event& event);
	bool OnFrameBufferSize(WindowResizedEvent& event);
	bool OnWindowClosed(WindowClosedEvent& event);

private:
	// constructors, properties, getters and setters
	EngineContext(Window* startingWindow = new Window(), Scene* startingScene = new Scene(), Renderer* startingRenderer = new ForwardRenderer());

	static EngineContext* m_Engine;
	entt::registry m_Registry;

	// properties
	bool m_IsRunning{ true };
	bool m_IsMinimized{ false };

	bool m_CamFlyMode{ false };
	bool m_FirstMouse{ false };
	float m_PrevMouseX{ 0.0f };
	float m_PrevMouseY{ 0.0f };

	float m_DeltaTime{ 0.0f };
	float m_LastFrameTime{ 0.0f };

	std::unique_ptr<Window> m_MainWindow;
	Scene* m_ActiveScene;
	Renderer* m_Renderer;

	ImGuiLayer* m_ImGuiLayer;

	// DebugConsole* _console;
	// methods

	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
	bool OnMouseScrolled(MouseScrolledEvent& event);
	bool OnMouseMoved(MouseMovedEvent& event);
	bool OnKeyPressedEvent(KeyPressedEvent& event);
	/*{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}*/
};

