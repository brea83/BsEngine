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

		void Draw();

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
		static void StaticDraw(GameObject& selected);
		void Draw(GameObject& selected);

		static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
		static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

		PlayerData ReadPlayerData() const { return m_Data; }

		void PointsOnKill(int pointsValue) 
		{ 
			m_Data.Score += pointsValue;
		}
		
		void IncrementKills() { m_Data.EnemiesDestroyed++; }

		void CollectPoints(int value) { m_Data.PointsCollected += value; m_Data.Score += value; }

		void OnLevelTrigger(int levelIndex);//std::filesystem::path nextLevelPath);

		void StartBoosting();
		void StopBoosting();
		void StartBreaking();
		void StopBreaking();

	private:
		static const std::string m_Name;
		bool m_ShowPlayerDataInDetailsView{ false };
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

		// attack component I think can be on the same component as the player script

		// movement Component unused in editor just now, going to find these in children on begin play
		GUID m_ReticleObjectID{ 0 };
		GameObject m_Reticle{};
		float m_BaseReticlePosZ{ 0.0f };
		float m_ReticleBoostPosZ{ 0.0f };
		float m_ReticleBreakPosZ{ 0.0f };

		void BindReticle(std::shared_ptr<Scene> scene, GameObject& hostObject);

		// reticle follower component unused in editor  just now, going to find these in children on begin play
		GUID m_ReticleFollowerID{ 0 };
		GameObject m_ReticleFollower{};

		std::string m_TrackFollowerTag = "TrackFollower";
		GUID m_TrackFollowerID{ 0 };
		GameObject m_TrackFollower{};

		void BindFollowers(std::shared_ptr<Scene> scene, GameObject& hostObject);

		float m_BaseTrackSpeed{ 1.0f };
		float m_BaseReticleSpeed{ 1.0f };
		float m_BaseReticleFollowerSpeed{ 1.0f };
		float m_BaseFollowZ{ -1.0f };

		bool m_IsBoosting{ false };
		bool m_IsBoostDecaying{ false };
		float m_BoostMultiplier{ 2.0f };
		float m_BoostDecayTime{ 1.0f };
		float m_BoostedFollowZ{ -0.5f };

		float m_AccumulatedBoostTime{ 0.0f };
		//float m_AccumulatedBoostDecay{ 0.0f };
		
		bool m_IsBreaking{ false };
		bool m_IsBreakDecaying{ false };
		float m_BreakSpeedMultiplier{ 0.25f };
		float m_BreakDecayTime{ 2.0f };
		float m_BreakingFollowZ{ -1.5f };

		float m_AccumulatedBreakTime{ 0.0f };
		//float m_AccumulatedBreakDecay{ 0.0f };

		void Update(GameObject& hostObject, float deltaTime);

		void LerpReticle(float deltaTime);
		void OrientTowardsReticle(float deltaTime);

		void SetNewSpeeds(float speedMult);

		friend class SplinePlayerInput;
	};

}