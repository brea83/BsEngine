#include "Scene.h"
#include "Graphics/Primitives/Renderable.h"
#include "Graphics/Camera.h"

Scene::Scene()
{ 
	_mainCamera = new Camera();
}

Scene::~Scene()
{
	// unload scene objects that do not persist across scenes
	if (!_objectsToRender.empty())
	{
		for (Renderable* renderObject : _objectsToRender)
		{
			delete renderObject;
		}
	}
}
