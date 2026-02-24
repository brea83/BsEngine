#pragma once

#include "Resources/FileStream.h"

namespace Pixie
{
	class GameObject;
	struct CollisionEvent;

	struct StoredScript
	{
		std::function<void(GameObject&)> AttachComponent;
		std::function<void(GameObject&)> RemoveComponent;
		std::function<void(GameObject&, GameObject&)> CopyComponent;
		std::function<void(StreamWriter*, const GameObject&)> Serialize;
		std::function<bool(StreamReader*, GameObject&)> Deserialize;

		std::function<void(GameObject&)> OnBeginPlay;
		std::function<void(GameObject&)> OnDestroy;
		std::function<void(GameObject&, float)> OnUpdate;
		std::function<void(GameObject&, CollisionEvent&)> OnCollision;
		std::function<void(GameObject&, CollisionEvent&, float)> OnCollisionOngoing;

		std::function<void(GameObject&) > Draw;
	};

// EXAMPLE SCRIPT CLASS
	//class ComponentScript
	//{
	//public:
	//	
	//	static void RegisterToScriptManager();
	//	static void AddMyComponentToGameObject(GameObject& hostObject);
	//	static void RemoveMyComponent(GameObject& hostObject);
	//	static void CopyComponent(GameObject& sourceObject, GameObject& destinationObject);
	//	static std::string_view GetName() { return m_Name; }
	//
	//	static void OnUpdate(GameObject& caller, float deltaTime);
	//	static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
	//	static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime);
	//	static void Draw(GameObject& selected);
	//
	//	static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
	//	static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

	//private:
	//	static const std::string m_Name;
	//	
	//};

}