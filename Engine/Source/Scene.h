#pragma once

#include <vector>
//#include <memory>

class Renderable;
class Camera;

class Scene
{
public:
	Scene();
	~Scene();
	void AddRenderable(Renderable* newRenderable) { _objectsToRender.push_back(newRenderable); }
	std::vector<Renderable*>& GetRenderables() { return _objectsToRender; }

	Camera* GetMainCamera() { return _mainCamera; }
private:
	Camera* _mainCamera;
	std::vector<Renderable*> _objectsToRender; 

	friend class SceneHierarchyPanel;
};

