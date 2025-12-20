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

	//TODO: update this to read in serialized data for loading scenes etc
	void GameObject::Init()
	{
		// send renderable components to graphics pipeline? 
		// or do those components do that themselves?
	}

	void GameObject::OnUpdate(float deltaTime)
	{
		//std::cout << "GameObject update, entity id: " << (int)m_EntityHandle << std::endl;
	}

	void GameObject::Serialize(StreamWriter* fileWriter, const GameObject& object)
	{

		fileWriter->WriteRaw<entt::entity>(object.m_EntityHandle);

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
		fileReader->ReadRaw<entt::entity>(object.m_SerializedID);
		std::vector<SerializableComponentID> components;
		fileReader->ReadArray<SerializableComponentID>(components);

		for (auto id : components)
		{
			if(id == SerializableComponentID::TagComponent)
				fileReader->ReadObject(object.GetOrAddComponent<TagComponent>());

			if(id== SerializableComponentID::NameComponent)
				fileReader->ReadObject(object.GetOrAddComponent<NameComponent>());

			if(id == SerializableComponentID::HeirarchyComponent)
				fileReader->ReadObject(object.GetOrAddComponent<HeirarchyComponent>());

			if(id == SerializableComponentID::TransformComponent)
				fileReader->ReadRaw<TransformComponent>(object.GetOrAddComponent<TransformComponent>());

			if(id == SerializableComponentID::MeshComponent)
				fileReader->ReadObject(object.GetOrAddComponent<MeshComponent>());

			if(id == SerializableComponentID::LightComponent)
				fileReader->ReadRaw<LightComponent>(object.GetOrAddComponent<LightComponent>());

			if(id == SerializableComponentID::CameraComponent)
				fileReader->ReadObject(object.GetOrAddComponent<CameraComponent>());

			if(id == SerializableComponentID::CameraController)
				fileReader->ReadObject(object.GetOrAddComponent<CameraController>());
		}

		return true;
	}

	TransformComponent& GameObject::GetTransform()
	{
		return m_Scene->GetRegistry().get_or_emplace<TransformComponent>(*this);
	}

	void GameObject::SetParent(entt::entity newParent, bool bSentFromAddChild)
	{
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		if (family.Parent != entt::null)
		{
			GameObject parentObject{ family.Parent, m_Scene };
			parentObject.RemoveChild(*this);
		}

		family.Parent = newParent;
		TransformComponent& transform = GetComponent<TransformComponent>();
		transform.ParentEntityHandle = newParent;

		if (!bSentFromAddChild)
		{
			GameObject parentObject{ family.Parent, m_Scene };
			parentObject.AddChild(*this);
		}
	}

	GameObject GameObject::GetParent()
	{
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		if (m_Scene->IsEntityValid(family.Parent))
		{
			return GameObject(family.Parent, m_Scene);
		}
		return GameObject();
	}

	std::vector<GameObject> GameObject::GetChildren()
	{
		std::vector<GameObject> children;
		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		if (family.Children.size() > 0)
		{
			for (auto entity : family.Children)
			{
				children.emplace_back( entity, m_Scene );
			}
		}
		return children;
	}

	void GameObject::UnParent(entt::entity grandParent, bool bKeepWorldPosition)
	{

		HeirarchyComponent& family = GetComponent<HeirarchyComponent>();
		TransformComponent& transform = GetComponent<TransformComponent>();
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
}