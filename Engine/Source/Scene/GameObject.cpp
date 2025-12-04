#include "BsPrecompileHeader.h"
#include "Scene/GameObject.h"
#define GLFW_INCLUDE_NONE
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Graphics/Primitives/Transform.h"
#include "Scene/Components/MeshComponent.h"
//#include "Scene/Components/CameraComponent.h"

static int s_NumGameObjects = 1;
GameObject::GameObject(entt::entity entity, Scene* scene)
	: Entity(entity, scene)
{}

GameObject::~GameObject()
{
	if (m_Children.size() > 0)
	{
		for (GameObject* child : m_Children)
		{
			child->UnParent(m_Parent);
		}
	}

}

//TODO: update this to read in serialized data for loading scenes etc
void GameObject::Init()
{
	// send renderable components to graphics pipeline? 
	// or do those components do that themselves?
}

//Transform& GetTransform()
//{
//	
//}

void GameObject::SetParent(GameObject* newParent, bool bSentFromAddChild)
{
	if (newParent == nullptr || m_Parent == newParent) return;

	if (m_Parent != nullptr)
	{
		m_Parent->RemoveChild(this);
	}

	m_Parent = newParent;
	//m_Transform->ParentTransform = m_Parent->GetTransform();

	if (!bSentFromAddChild)
	{
		m_Parent->AddChild(this);
	}
}


void GameObject::UnParent(GameObject* grandParent, bool bKeepWorldPosition)
{
	//m_Transform->UnParent();
	m_Parent = grandParent;
}

void GameObject::AddChild(GameObject* child, bool bSentFromSetParent)
{
	if (child == nullptr) return;

	if (std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end())
	{
		// already is a child. do nothing
		return;
	}

	m_Children.push_back(child);

	if (!bSentFromSetParent)
	{
		child->SetParent(this, true);
	}
}

void GameObject::RemoveChild(GameObject* child)
{
	if (child == nullptr) return;
	auto foundItterator = std::find(m_Children.begin(), m_Children.end(), child);

	if (foundItterator != m_Children.end())
	{
		m_Children.erase(foundItterator);
	}
}