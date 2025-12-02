#include "BsPrecompileHeader.h"
#include "Scene/GameObject.h"
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Graphics/Primitives/Transform.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Components/CameraComponent.h"

static int s_NumGameObjects = 1;
GameObject::GameObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Name(name + " " + std::to_string(s_NumGameObjects++)), _transform(std::make_shared<Transform>(position, rotation, scale))
{}

GameObject::~GameObject()
{
	for (auto pair : _components)
	{
		std::shared_ptr<MeshComponent> model = std::dynamic_pointer_cast<MeshComponent>(pair.second);
		if (model)
		{
			EngineContext::GetEngine()->GetScene()->RemoveRenderable(model);
		}
	}

	if (_parent == nullptr && _children.size() > 0)
	{
		for (GameObject* child : _children)
		{
			child->UnParent();
		}
	}
}

void GameObject::SetParent(GameObject* newParent, bool bSentFromAddChild)
{
	if (newParent == nullptr || _parent == newParent) return;

	if (_parent != nullptr)
	{
		_parent->RemoveChild(this);
	}

	_parent = newParent;
	_transform->ParentTransform = _parent->GetTransform();

	if (!bSentFromAddChild)
	{
		_parent->AddChild(this);
	}
}


void GameObject::UnParent(bool bKeepWorldPosition)
{
	_transform->UnParent();
	_parent = nullptr;
}

void GameObject::AddChild(GameObject* child, bool bSentFromSetParent)
{
	if (child == nullptr) return;

	if (std::find(_children.begin(), _children.end(), child) != _children.end())
	{
		// already is a child. do nothing
		return;
	}

	_children.push_back(child);

	if (!bSentFromSetParent)
	{
		child->SetParent(this, true);
	}
}

void GameObject::RemoveChild(GameObject* child)
{
	if (child == nullptr) return;
	auto foundItterator = std::find(_children.begin(), _children.end(), child);

	if (foundItterator != _children.end())
	{
		_children.erase(foundItterator);
	}
}

//TODO: update this to read in serialized data for loading scenes etc
void GameObject::Init()
{
	// send renderable components to graphics pipeline? 
	// or do those components do that themselves?
}

void GameObject::OnComponentAdded(std::shared_ptr<Component> component)
{
	if (std::dynamic_pointer_cast<MeshComponent>(component))
	{
		EngineContext::GetEngine()->GetScene()->AddRenderable(std::dynamic_pointer_cast<MeshComponent>(component));
	}

	//if (std::dynamic_pointer_cast<CameraComponent>(component))
	//{
	//	EngineContext::GetEngine()->GetScene()->AddCamera(std::dynamic_pointer_cast<CameraComponent>(component));
	//}
}

void GameObject::OnComponentRemoved(std::shared_ptr<Component> component)
{
	//Scene* scene = EngineContext::GetEngine()->GetScene();
	if (std::dynamic_pointer_cast<MeshComponent>(component))
	{
		EngineContext::GetEngine()->GetScene()->RemoveRenderable(std::dynamic_pointer_cast<MeshComponent>(component));
	}

	/*if (std::dynamic_pointer_cast<CameraComponent>(component))
	{
		if (!EngineContext::GetEngine()->GetScene()->TryRemoveCamera(std::dynamic_pointer_cast<CameraComponent>(component)))
		{
			_components[typeid(CameraComponent).hash_code()] = component;
		}
	}*/
}
