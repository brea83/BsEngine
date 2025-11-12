#pragma once
#include "BsPrecompileHeader.h"
#include "../Shaders/Shader.h"
#include <glm/glm.hpp>


class Transform;

class Renderable
{
public:

	Renderable(const std::string& name = "Default Name");
	virtual ~Renderable();

	std::string Name;

	virtual Transform* GetTransform() { return _transform; }
	virtual void Render(Shader& currentShader) = 0;
protected:
	//properties
	Transform* _transform{ nullptr };

	// methods
	virtual void Init() = 0;
};
//static const std::array<int, 2> VetexDataOffsets{ 0, sizeof(glm::vec3) };