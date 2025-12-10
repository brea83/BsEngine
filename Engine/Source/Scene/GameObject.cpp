#include "BsPrecompileHeader.h"
#include "Scene/GameObject.h"
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Graphics/Primitives/Transform.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Components/CameraComponent.h"

namespace Pixie
{
	static int s_NumGameObjects = 1;
	GameObject::GameObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: Name(name + " " + std::to_string(s_NumGameObjects++)), m_Transform(std::make_shared<Transform>(position, rotation, scale))
	{}

	GameObject::~GameObject()
	{
		for (auto pair : m_Components)
		{
			std::shared_ptr<MeshComponent> model = std::dynamic_pointer_cast<MeshComponent>(pair.second);
			if (model)
			{
				EngineContext::GetEngine()->GetScene()->RemoveRenderable(model);
			}
		}

		if (m_Parent == nullptr && m_Children.size() > 0)
		{
			for (GameObject* child : m_Children)
			{
				child->UnParent();
			}
		}
	}

	void GameObject::SetParent(GameObject* newParent, bool bSentFromAddChild)
	{
		if (newParent == nullptr || m_Parent == newParent) return;

		if (m_Parent != nullptr)
		{
			m_Parent->RemoveChild(this);
		}

		m_Parent = newParent;
		m_Transform->ParentTransform = m_Parent->GetTransform();

		if (!bSentFromAddChild)
		{
			m_Parent->AddChild(this);
		}
	}


	void GameObject::UnParent(bool bKeepWorldPosition)
	{
		m_Transform->UnParent();
		m_Parent = nullptr;
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
				m_Components[typeid(CameraComponent).hash_code()] = component;
			}
		}*/
	}
}