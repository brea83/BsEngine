#include "BsPrecompileHeader.h"
#include "Scene.h"
#include "Graphics/Primitives/Renderable.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Camera.h"

Scene::Scene()
{ 
	_mainCamera = new Camera();
	//NextUID = 0;
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

void Scene::CreateCube()
{
	_objectsToRender.emplace_back(new Cube());
}

void Scene::RemoveRenderable(Renderable* objToRemove)
{
	auto foundItterator = std::find(_objectsToRender.begin(), _objectsToRender.end(), objToRemove);

	if (foundItterator != _objectsToRender.end())
	{
		std::cout << "FOUND OBJECT: " << objToRemove->Name << std::endl;
		_objectsToRender.erase(foundItterator);
		delete objToRemove;
	}
	else
	{
		std::cout << objToRemove->Name << ", NOT FOUND" << std::endl;
	}
}
