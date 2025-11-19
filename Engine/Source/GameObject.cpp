#include "BsPrecompileHeader.h"
#include "GameObject.h"
#include "Graphics/Primitives/Transform.h"

GameObject::GameObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Name(name), _transform(std::make_shared<Transform> (position, rotation, scale))
{}

//TODO: update this to read in serialized data for loading scenes etc
void GameObject::Init()
{
}
