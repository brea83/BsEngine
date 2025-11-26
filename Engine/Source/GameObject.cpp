#include "BsPrecompileHeader.h"
#include "GameObject.h"
#include "EngineContext.h"
#include "Scene.h"
#include "Graphics/Primitives/Transform.h"
#include "Graphics/Model.h"

static int s_NumGameObjects = 1;
GameObject::GameObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Name(name + " " + std::to_string(s_NumGameObjects++)), _transform(std::make_shared<Transform>(position, rotation, scale))
{}

GameObject::~GameObject()
{
	for (auto pair : _components)
	{
		std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(pair.second);
		if (model)
		{
			EngineContext::GetEngine()->GetScene()->RemoveRenderable(model);
		}
	}
}

void GameObject::SetParent(GameObject* newParent)
{
}

void GameObject::AddChild(GameObject* child)
{

}

//TODO: update this to read in serialized data for loading scenes etc
void GameObject::Init()
{
	// send renderable components to graphics pipeline? 
	// or do those components do that themselves?
}

void GameObject::OnComponentAdded(std::shared_ptr<Component> component)
{
	if (std::dynamic_pointer_cast<Model>(component))
	{
		EngineContext::GetEngine()->GetScene()->AddRenderable(std::dynamic_pointer_cast<Model>(component));
	}
}
