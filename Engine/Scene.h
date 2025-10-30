#pragma once

#include <vector>
//#include <memory>

class Renderable;

class Scene
{
public:
	Scene();
	~Scene();
	void AddRenderable(Renderable* newRenderable) { _objectsToRender.push_back(newRenderable); }
	std::vector<Renderable*>& GetRenderables() { return _objectsToRender; }

	//main camera
	//frame buffer?
	
private:
	std::vector<Renderable*> _objectsToRender; 
};

