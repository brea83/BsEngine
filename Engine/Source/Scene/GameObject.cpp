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
{}

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

void GameObject::SetParent(entt::entity newParent, bool bSentFromAddChild)
{
	HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
	if (family.Parent != entt::null)
	{
		GameObject parentObject{ family.Parent, m_Scene };
		parentObject.RemoveChild(*this);
	}

	family.Parent = newParent;
	Transform& transform = GetComponent<Transform>();
	transform.ParentEntityHandle = newParent;

	if (!bSentFromAddChild)
	{
		GameObject parentObject{ family.Parent, m_Scene };
		parentObject.AddChild(*this);
	}
}


void GameObject::UnParent(entt::entity grandParent, bool bKeepWorldPosition)
{

	HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
	Transform& transform = GetComponent<Transform>();
	transform.UnParent(m_Scene, *this, grandParent);
	family.Parent = grandParent;
}

void GameObject::AddChild(entt::entity child, bool bSentFromSetParent)
{
	if (!m_Scene->GetRegistry().valid(child)) return;

	HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
	auto itterator = std::find(family.Children.begin(), family.Children.end(), child);
	if (itterator != family.Children.end())
	{
		// already is a child. do nothing
		return;
	}

	family.Children.push_back(child);

	if (!bSentFromSetParent)
	{
		GameObject childObject{ child, m_Scene };
		childObject.SetParent(*this, true);
	}
}

void GameObject::RemoveChild(entt::entity child)
{
	if (!m_Scene->GetRegistry().valid(child)) return;
	HeirarchyComponent& family = GetComponent<HeirarchyComponent>();

	if (family.Children.empty()) return;
	auto itterator = std::find(family.Children.begin(), family.Children.end(), child);
	if (itterator != family.Children.end())
	{
		family.Children.erase(itterator);
	}
}