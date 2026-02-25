#pragma once
#include "Scene/Components/IComponentScript.h"
#include "Scene/GameObject.h"

namespace Pixie
{
	struct PlayerData
	{
		PlayerData() = default;
		PlayerData(const PlayerData&) = default;

		int Score{ 0 };
		int PointsCollected{ 0 };
		int EnemiesDestroyed{ 0 };
	};

	class Player
	{
	public:
		Player() = default;
		Player(const Player&) = default;

		static void RegisterToScriptManager();
		static void Attach(GameObject& hostObject);
		static void Remove(GameObject& hostObject);
		static void Copy(GameObject& sourceObject, GameObject& destinationObject);
		static std::string_view GetName() { return m_Name; }

		static void OnBeginPlay(GameObject& hostObject);
		static void OnUpdate(GameObject& caller, float deltaTime);
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
		static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime);
		static void Draw(GameObject& selected);

		static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
		static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

		PlayerData ReadPlayerData() const { return m_Data; }

		void PointsOnKill(int pointsValue) 
		{ 
			m_Data.Score += pointsValue;
		}
		
		void IncrementKills() { m_Data.EnemiesDestroyed++; }

		void CollectPoints(int value) { m_Data.PointsCollected += value; m_Data.Score += value; }

	private:
		static const std::string m_Name;
		
		PlayerData m_Data{};

		GUID m_PointCollectorID{0};
		GameObject m_ObjectWithPointCollector{};

		// point collector component
		// attack component
		// damageable component
		// OnDeath()
	};

}