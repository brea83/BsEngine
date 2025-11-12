#include "BsPrecompileHeader.h"
#include "Renderable.h"
#include "Transform.h"

Renderable::Renderable(const std::string& name)
	: Name(name)
{
	_transform = new Transform();
}

Renderable::~Renderable()
{}
