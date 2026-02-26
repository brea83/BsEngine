#pragma once
#include "Scene/Components/IComponentScript.h"
#include "Scene/GameObject.h"

namespace Pixie
{
	struct PlayerData
	{
		PlayerData() = default;
		PlayerData(const PlayerData&) = default;

		int Score{ -1 };
		int PointsCollected{ 0 };
		int EnemiesDestroyed{ 0 };

		static void Serialize(StreamWriter* stream, const PlayerData& data)
		{
			stream->WriteRaw<int>(data.Score);
			stream->WriteRaw<int>(data.PointsCollected);
			stream->WriteRaw<int>(data.EnemiesDestroyed);
		}
		static bool Deserialize(StreamReader* stream, PlayerData& data)
		{
			stream->ReadRaw<int>(data.Score);
			stream->ReadRaw<int>(data.PointsCollected);
			stream->ReadRaw<int>(data.EnemiesDestroyed);
			return true;
		}
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

		void OnLevelTrigger(std::filesystem::path nextLevelPath);

	private:
		static const std::string m_Name;
		
		PlayerData m_Data{};

		// point collector component
		GUID m_PointCollectorID{0};
		GameObject m_ObjectWithPointCollector{};
		void BindPointsCallback(std::shared_ptr<Scene> scene, GameObject& hostObject);

		// damageable component
		GUID m_DamageableID{ 0 };
		GameObject m_ObjectWithDamageable{};

		void BindDeathCallback(std::shared_ptr<Scene> scene, GameObject& hostObject);
		void OnDeath(GUID killerID);

		// attack component
		GUID m_AttackID{ 0 };
	};

}