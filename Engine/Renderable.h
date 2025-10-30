#pragma once
#include "Shaders/Shader.h"

class Renderable
{
public:
	virtual void Render(Shader& currentShader) = 0;

};

