#pragma once

#include <vector>

class Renderable;

class Scene
{
public:
	std::vector<Renderable*> objectsToRender; 
};

