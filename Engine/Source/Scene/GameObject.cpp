#include "BsPrecompileHeader.h"
#include "GameObject.h"
#define GLFW_INCLUDE_NONE
#include "EngineContext.h"
#include "Scene.h"
#include "Scene/Components/Component.h"
#include "Components/Transform.h"
#include "Components/MeshComponent.h"
#include "Components/CameraController.h"

namespace Pixie
{
	static int s_NumGameObjects = 1;
	GameObject::GameObject(entt::entity entity, Scene* scene)
		: Entity(entity, scene)
	{}

	GameObject::~GameObject()
	{}

	void GameObject::OnUpdate(float deltaTime)
	{
		//Logger::Log(LOG_TRACE, "GameObject update, entity id: {}", (int)m_EntityHandle);
	}

	void GameObject::Serialize(StreamWriter* fileWriter, const GameObject& object)
	{
		if (!object.HasCompoenent<IDComponent>())
		{
			Logger::Log(LOG_ERROR, "Error: trying to serialize a gameobject with no GUID");
			return;
		}

		uint64_t guid = (uint64_t)object.GetGUID();
		fileWriter->WriteRaw<uint64_t>(guid);

		TagComponent* tag = object.TryGetComponent<TagComponent>();
		NameComponent* name = object.TryGetComponent<NameComponent>();
		HeirarchyComponent* heirarchy = object.TryGetComponent<HeirarchyComponent>();
		TransformComponent* transform = object.TryGetComponent<TransformComponent>();
		MeshComponent* mesh = object.TryGetComponent<MeshComponent>();
		LightComponent* light = object.TryGetComponent<LightComponent>();
		CameraComponent* camera = object.TryGetComponent<CameraComponent>();
		CameraController* camController = object.TryGetComponent<CameraController>();

		std::vector<SerializableComponentID> components;
		if (tag) components.push_back(SerializableComponentID::TagComponent);
		if (name) components.push_back(SerializableComponentID::NameComponent);
		if (heirarchy) components.push_back(SerializableComponentID::HeirarchyComponent);
		if (transform) components.push_back(SerializableComponentID::TransformComponent);
		if (mesh) components.push_back(SerializableComponentID::MeshComponent);
		if (light) components.push_back(SerializableComponentID::LightComponent);
		if (camera) components.push_back(SerializableComponentID::CameraComponent);
		if (camController) components.push_back(SerializableComponentID::CameraController);

		fileWriter->WriteArray<SerializableComponentID>(components);

		for (auto id : components)
		{
			if (id == SerializableComponentID::TagComponent)
				fileWriter->WriteObject(object.GetComponent<TagComponent>());

			if (id == SerializableComponentID::NameComponent)
				fileWriter->WriteObject(object.GetComponent<NameComponent>());

			if (id == SerializableComponentID::HeirarchyComponent)
				fileWriter->WriteObject(object.GetComponent<HeirarchyComponent>());

			if (id == SerializableComponentID::TransformComponent)
				fileWriter->WriteRaw<TransformComponent>(object.GetComponent<TransformComponent>());

			if (id == SerializableComponentID::MeshComponent)
				fileWriter->WriteObject(object.GetComponent<MeshComponent>());

			if (id == SerializableComponentID::LightComponent)
				fileWriter->WriteRaw<LightComponent>(object.GetComponent<LightComponent>());

			if (id == SerializableComponentID::CameraComponent)
				fileWriter->WriteObject(object.GetComponent<CameraComponent>());

			if (id == SerializableComponentID::CameraController)
				fileWriter->WriteObject(object.GetComponent<CameraController>());
		}

	}

	bool GameObject::Deserialize(StreamReader* fileReader, GameObject& object)
	{
		uint64_t guid;
		fileReader->ReadRaw<uint64_t>(guid);

		object.GetOrAddComponent<IDComponent>().ID = guid;

		std::vector<SerializableComponentID> components;
		fileReader->ReadArray<SerializableComponentID>(components);

		for (auto id : components)
		{
			if (id == SerializableComponentID::TagComponent)
			{
				fileReader->ReadObject(object.GetOrAddComponent<TagComponent>());
				continue;
			}

			if (id == SerializableComponentID::NameComponent)
			{
				fileReader->ReadObject(object.GetOrAddComponent<NameComponent>());
				continue;
			}

			if (id == SerializableComponentID::HeirarchyComponent)
			{
				fileReader->ReadObject(object.GetOrAddComponent<HeirarchyComponent>());
				continue;
			}

			if (id == SerializableComponentID::TransformComponent)
			{
				fileReader->ReadRaw<TransformComponent>(object.GetOrAddComponent<TransformComponent>());
				continue;
			}

			if (id == SerializableComponentID::MeshComponent)
			{
				fileReader->ReadObject(object.GetOrAddComponent<MeshComponent>());
				continue;
			}

			if (id == SerializableComponentID::LightComponent)
			{
				fileReader->ReadRaw<LightComponent>(object.GetOrAddComponent<LightComponent>());
				continue;
			}

			if (id == SerializableComponentID::CameraComponent)
			{
				fileReader->ReadObject(object.GetOrAddComponent<CameraComponent>());
				continue;
			}

			if (id == SerializableComponentID::CameraController)
			{
				fileReader->ReadObject(object.GetOrAddComponent<CameraController>());
				continue;
			}
		}

		return true;
	}

	TransformComponent& GameObject::GetTransform()
	{
		return m_Scene->GetRegistry().get_or_emplace<TransformComponent>(*this);
	}

	void GameObject::SetParentNone()
	{
		GameObject parent = GetParent();
		if (parent)
		{
			parent.RemoveChild(*this);
		}

		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		family.Parent = 0;
	}

	void GameObject::SetParent(GameObject& newParent, bool bSentFromAddChild)
	{
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		if (family.Parent != 0)
		{
			
			GameObject parentObject = m_Scene->FindGameObjectByGUID(family.Parent);
			parentObject.RemoveChild(*this);
		}

		family.Parent = newParent.GetGUID();
		TransformComponent& transform = GetComponent<TransformComponent>();
		transform.m_ParentGuid = newParent.GetGUID();

		if (!bSentFromAddChild)
		{
			GameObject parentObject = m_Scene->FindGameObjectByGUID(family.Parent);
			if(parentObject)
				parentObject.AddChild(*this);
		}
	}

	GameObject GameObject::GetParent()
	{
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		if (family.Parent != 0)
		{
			return m_Scene->FindGameObjectByGUID(family.Parent);
		}
		return GameObject();
	}

	std::vector<GameObject> GameObject::GetChildren()
	{
		std::vector<GameObject> children;
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		if (family.Children.size() > 0)
		{
			for (auto guid : family.Children)
			{
				GameObject child = m_Scene->FindGameObjectByGUID(guid);
				if(child)
					children.push_back(child);
			}
		}
		return children;
	}

	//void GameObject::UnParent(GameObject grandParent, bool bKeepWorldPosition)
	//{
	//	
	//	HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
	//	TransformComponent& transform = GetComponent<TransformComponent>();
	//	transform.UnParent(m_Scene, *this, grandParent);
	//	family.Parent = grandParent.GetGUID();
	//}

	void GameObject::AddChild(GameObject& child, bool bSentFromSetParent)
	{
		if (!m_Scene->GetRegistry().valid(child)) return;

		//m_Scene->FindGameObjectByGUID(guid);
		GUID childGuid = child.GetGUID();
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		auto itterator = std::find(family.Children.begin(), family.Children.end(), childGuid);

		if (itterator != family.Children.end())
		{
			// already is a child. do nothing
			return;
		}

		family.Children.push_back(childGuid);

		if (!bSentFromSetParent)
		{
			child.SetParent(*this, true);
		}
	}

	void GameObject::RemoveChild(GameObject& child)
	{
		if (!m_Scene->GetRegistry().valid(child)) return;
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();

		if (family.Children.empty()) return;

		GUID childGuid = child.GetGUID();
		auto itterator = std::find(family.Children.begin(), family.Children.end(), childGuid);
		if (itterator != family.Children.end())
		{
			family.Children.erase(itterator);
		}
	}
}