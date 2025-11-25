#pragma once
#include "BsPrecompileHeader.h"
#include "../Shaders/Shader.h"
#include <glm/glm.hpp>
#include "Resource.h"
#include "Graphics/Primitives/Transform.h"

class Renderable: public Resource
{
public:

	Renderable(/*unsigned int uid, */const std::string& name = "Default Name");
	virtual ~Renderable();

	std::string Name;

	virtual void Render(Shader& currentShader) = 0;

protected:
	// methods
	virtual void Init() = 0;
};
