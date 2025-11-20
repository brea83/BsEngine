#include "BsPrecompileHeader.h"
#include "GameObject.h"
#include "Graphics/Primitives/Transform.h"

GameObject::GameObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Name(name), _transform(std::make_shared<Transform> (position, rotation, scale))
{}

void GameObject::SetParent(std::shared_ptr<GameObject> newParent)
{}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
}

//TODO: update this to read in serialized data for loading scenes etc
void GameObject::Init()
{
	// send renderable components to graphics pipeline? 
	// or do those components do that themselves?
}
