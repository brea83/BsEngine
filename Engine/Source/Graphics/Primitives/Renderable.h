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

	//virtual unsigned int GetUID() const { return _uid; }
	virtual std::shared_ptr<Transform> GetTransform() { return _transform; }
	virtual void Render(Shader& currentShader) = 0;
	virtual void Render(Shader& currentShader, std::shared_ptr<Transform> transform) = 0;

protected:
	//properties
	std::shared_ptr<Transform> _transform{ nullptr };
	//unsigned int _uid;
	// methods
	virtual void Init() = 0;
};
//static const std::array<int, 2> VetexDataOffsets{ 0, sizeof(glm::vec3) };