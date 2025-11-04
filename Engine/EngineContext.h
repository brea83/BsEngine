#pragma once
//#include <GLFW/glfw3.h>
//#include "Window.h"

class Renderer;
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


	void SetScene(Scene* newScene) { _activeScene = newScene; }
	Scene* GetScene() { return _activeScene; }

	// methods
	void Draw();
	void DrawConsole();

	// Window event callbacks
	void OnFrameBufferSize(int width, int height);
	void OnGlfwInput(GLFWwindow* window);
private:
	// constructors, properties, getters and setters
	EngineContext();
	//EngineContext(/*Scene* scene, Renderer* renderer,*/ Window* window/*, DebugConsole* console */ );

	static EngineContext* _engine;

	// properties
	Window* _mainWindow;
	Scene* _activeScene;
	Renderer* _renderer;
	// DebugConsole* _console;
	// methods
};

