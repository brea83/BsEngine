#include "BsPrecompileHeader.h"
#include "Renderable.h"

namespace Pixie
{
	Renderable::Renderable(const std::string& name)
		: Resource(ResourceType::Renderable), Name(name)
	{

	}

	Renderable::~Renderable()
	{

	}
}