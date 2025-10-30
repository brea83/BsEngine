#include "Scene.h"
#include "Renderable.h"

Scene::Scene()
{ }

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
