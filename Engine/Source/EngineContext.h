#pragma once
//#include <GLFW/glfw3.h>
#include "GlfwWrapper.h"
#include "Scene.h"
#include "Graphics/Renderers/ForwardRenderer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Editor/ImGuiLayer.h"

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

	Window& GetWindow() { return *_mainWindow; }
	GLFWwindow* GetGlfwWindow() { return _mainWindow->GetGlfwWindow(); }

	void SetScene(Scene* newScene) { _activeScene = newScene; }
	Scene* GetScene() { return _activeScene; }

	Renderer* GetRenderer() { return _renderer; }

	// methods
	bool IsRunning()const { return _bRunning; }
	void StopApplication() { _bRunning = false; }
	void Update();
	void Draw();
	void DrawConsole();

	// Window event callbacks
	void OnEvent(Event& event);
	bool OnFrameBufferSize(WindowResizedEvent& event);
	bool OnWindowClosed(WindowClosedEvent& event);

private:
	// constructors, properties, getters and setters
	EngineContext(Window* startingWindow = new Window(), Scene* startingScene = new Scene(), Renderer* startingRenderer = new ForwardRenderer());

	static EngineContext* _engine;

	// properties
	bool _bRunning{ true };
	bool _bMinimized{ false };
	std::unique_ptr<Window> _mainWindow;
	Scene* _activeScene;
	Renderer* _renderer;

	ImGuiLayer* _imGuiLayer;

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

