#pragma once
#include "GUID.h"
#include <string>
#include <vector>
namespace Pixie
{
	class GameObject;
	struct CollisionEvent;

	class StreamWriter;
	class StreamReader;

	struct Attack
	{
		int Damage;
	};

	class Damageable
	{
	public:
		static void RegisterToScriptManager();
		static void AddMyComponentToGameObject(GameObject& hostObject);
		static void RemoveMyComponent(GameObject& hostObject);
		static void CopyComponent(GameObject& sourceObject, GameObject& destinationObject);
		static std::string_view GetName() { return m_Name; }

		static void OnUpdate(GameObject& caller, float deltaTime);
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
		static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime);
		static void Draw(GameObject& selected);

		static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
		static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

	private:
		static const std::string m_Name;

		std::vector<std::string> m_TagsThatDamageThis;
		int m_MaxHealth;
		int m_CurrentHealth;

		int m_IFrames;
		int m_AccumulatedIFrames;

		std::unordered_map<GUID, int> m_DamageSourcesThisFrame;

		bool m_IsInvulnerable;

		static GameObject ExtractOtherObject(GameObject thisObject, CollisionEvent& collision);
		bool TestCollisionValid(GameObject& thisObject, GameObject& other);

		// occurs durring collision evaluation
		void CollectDamage(GameObject& thisObject, GameObject& other);

		void Update(GameObject& thisObject, float deltaTime);

		// occures durring update
		void TakeDamage();

	};

}