#pragma once
#include "GUID.h"
#include "../CombatComponents.h"
#include <string>
#include <vector>
namespace Pixie
{
	class GameObject;
	struct CollisionEvent;

	class StreamWriter;
	class StreamReader;

	
	class Damageable
	{
	public:
		static void RegisterToScriptManager();
		static void AddMyComponentToGameObject(GameObject& hostObject);
		static void RemoveMyComponent(GameObject& hostObject);
		static void CopyComponent(GameObject& sourceObject, GameObject& destinationObject);
		static std::string_view GetName() { return m_Name; }

		static void OnBeginPlay(GameObject& caller);
		static void OnUpdate(GameObject& caller, float deltaTime);
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
		static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime);
		static void Draw(GameObject& selected);

		static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
		static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

		void AddOnDeathCallback(const std::string& name, std::function<void(GUID)> callback);
		void RemoveOnDeathCallback(const std::string& name, std::function<void(GUID)> callback);
		void ClearAllCallbacks();
	private:
		static const std::string m_Name;

		std::vector<std::string> m_TagsThatDamageThis{ "Player" };
		int m_MaxHealth{ 2 };
		int m_CurrentHealth{ 2 };

		int m_IFrames{ 0 };
		int m_AccumulatedIFrames{ 0 };

		std::unordered_map<GUID, int> m_DamageSourcesThisFrame;
		GUID m_LastDamageSource{ 0 };
		std::unordered_map< std::string, std::function<void(GUID)>> m_OnDeathCallbacks;

		bool m_IsInvulnerable{ false };

		void Reset();

		
		bool TestCollisionValid(GameObject& thisObject, GameObject& other);
		// occurs durring collision evaluation
		void CollectDamage(GameObject& thisObject, GameObject& other);

		void Update(GameObject& thisObject, float deltaTime);
		// occures durring update
		void TakeDamage();
		void OnDeath(GameObject& thisObject, GUID killerId);

	};

}