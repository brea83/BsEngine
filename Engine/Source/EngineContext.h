#pragma once
//#include <GLFW/glfw3.h>
#include "GlfwWrapper.h"
#include "Scene.h"
#include "Graphics/Renderers/ForwardRenderer.h"

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

	//TODO: FIX HOW WINDOW AND ENGINE ARE CONNECTED THIS IS AN AWFUL FUNCTION TO JUST HAVE BE PUBLIC
	//void SetMainWindow(Window* window) { _mainWindow = window; }
	Window* GetMainWindow() { return _mainWindow; }

	void SetScene(Scene* newScene) { _activeScene = newScene; }
	Scene* GetScene() { return _activeScene; }

	// methods
	bool IsRunning()const { return _isRunning; }
	void StopApplication() { _isRunning = false; }
	void Update();
	void Draw();
	void DrawConsole();

	// Window event callbacks
	void OnFrameBufferSize(int width, int height);
	void OnGlfwInput(GLFWwindow* window);

private:
	// constructors, properties, getters and setters
	EngineContext(Window* startingWindow = new Window(), Scene* startingScene = new Scene(), Renderer* startingRenderer = new ForwardRenderer());
	//EngineContext(/*Scene* scene, Renderer* renderer,*/ Window* window/*, DebugConsole* console */ );

	static EngineContext* _engine;

	// properties
	bool _isRunning{ true };
	Window* _mainWindow;
	Scene* _activeScene;
	Renderer* _renderer;
	// DebugConsole* _console;
	// methods
};

