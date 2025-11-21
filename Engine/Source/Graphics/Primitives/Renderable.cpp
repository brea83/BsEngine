#include "BsPrecompileHeader.h"
#include "Renderable.h"
#include "Transform.h"

Renderable::Renderable(/*unsigned int uid,*/ const std::string& name )
	: Resource(ResourceType::Renderable), /*_uid(uid),*/ Name(name)
{
	//_transform = std::make_shared<Transform>();
}

Renderable::~Renderable()
{
	
}
