#pragma once

class Renderer;
class Scene;
//class DebugConsole;

class EngineContext
{
public:
	// constructors, properties, getters and setters
	EngineContext();
	EngineContext(Scene* scene, Renderer* renderer/*, DebugConsole* console */ );
	void SetScene(Scene* newScene) { _activeScene = newScene; }
	Scene* GetScene() { return _activeScene; }

	// methods
	void Draw();
	void DrawConsole();
private:
	// properties
	Scene* _activeScene;
	Renderer* _renderer;
	// DebugConsole* _console;
	// methods
};

