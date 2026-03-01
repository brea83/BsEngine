#include "Player.h"
#include "Scene/GameObject.h"
#include "Physics/PhysicsEngine.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"
#include "EngineContext.h"

#include "../ExampleGame.h"
#include "../CombatComponents.h"
#include "../CollisionBehavior/PointCollector.h"
#include "../CollisionBehavior/Damageable.h"

namespace Pixie
{
	const std::string Player::m_Name = "Player Component";
	void Player::RegisterToScriptManager()
	{
		Pixie::ScriptManager* scripts = Pixie::ScriptManager::GetInstance();

		using std::placeholders::_1;
		using std::placeholders::_2;
		using std::placeholders::_3;
		Pixie::StoredScript newScript;
		newScript.AttachComponent = std::bind(&Player::Attach, _1);
		newScript.CopyComponent = std::bind(&Player::Copy, _1, _2);
		newScript.RemoveComponent = std::bind(&Player::Remove, _1);
		newScript.Serialize = std::bind(&Player::Serialize, _1, _2);
		newScript.Deserialize = std::bind(&Player::Deserialize, _1, _2);
		newScript.Draw = std::bind(&Player::StaticDraw, _1);

		newScript.OnBeginPlay = std::bind(&Player::OnBeginPlay, _1);
		newScript.OnUpdate = std::bind(&Player::OnUpdate, _1, _2);

		scripts->TryStoreScript(m_Name, newScript);
	}

	void Player::Attach(GameObject& hostObject)
	{
		hostObject.AddComponent<Player>();
	}
	void Player::Remove(GameObject& hostObject)
	{
		hostObject.RemoveComponent<Player>();
	}
	void Player::Copy(GameObject& sourceObject, GameObject& destinationObject)
	{
		if (!sourceObject.HasCompoenent<Player>())
			return;
		Player& source = sourceObject.GetComponent<Player>();

		destinationObject.AddOrReplaceComponent<Player>(source);
	}

	void Player::OnBeginPlay(GameObject& hostObject)
	{
		Player& player = hostObject.GetComponent<Player>();
		std::shared_ptr<Scene> scene = hostObject.GetScene();

		player.m_Data.Score = 0;
		player.BindPointsCallback(scene, hostObject);
		player.BindDeathCallback(scene, hostObject);
		
		// try to find a movement component in children or parent;
		// this is specific to this spline game set up where there is only one move component and it's on the reticle
		player.BindReticle(scene, hostObject);
		player.BindFollowers(scene, hostObject);
	}


	void Player::BindPointsCallback(std::shared_ptr<Scene> scene, GameObject& hostObject)
	{
		if (m_PointCollectorID != 0)
		{
			m_ObjectWithPointCollector = scene->FindGameObjectByGUID(m_PointCollectorID);
			if (m_ObjectWithPointCollector)
			{
				PointCollector& collector = m_ObjectWithPointCollector.GetComponent<PointCollector>();
				using std::placeholders::_1;
				collector.SetOnPointsCollectedCallback(std::bind(&Player::CollectPoints, this, _1));
			}
		}
		else
		{
			// try to find a point collector in children or parent;
			PointCollector* inParent = hostObject.GetComponentInParent<PointCollector>();
			if (inParent)
			{
				m_ObjectWithPointCollector = hostObject.GetParent();
				m_PointCollectorID = m_ObjectWithPointCollector.GetGUID();
				using std::placeholders::_1;
				inParent->SetOnPointsCollectedCallback(std::bind(&Player::CollectPoints, this, _1));
			}
			else
			{
				std::vector<GameObject> inChildren = hostObject.GetChildrenWithComponent<PointCollector>();

				if (!inChildren.empty())
				{
					GameObject child = inChildren.front();
					PointCollector& collector = child.GetComponent<PointCollector>();
					m_ObjectWithPointCollector = child;

					m_PointCollectorID = child.GetGUID();

					using std::placeholders::_1;
					collector.SetOnPointsCollectedCallback(std::bind(&Player::CollectPoints, this, _1));
				}
			}
		}
	}

	void Player::BindReticle(std::shared_ptr<Scene> scene, GameObject& hostObject)
	{
		// try to find a movement component in children or parent;
		// this is specific to this spline game set up where the reticle is the only object with both a player input component and a movecomponent
		entt::registry& registry = scene->GetRegistry();

		for (auto&& [entity, inputComponent, movement, transform] : registry.view<PlayerInputComponent, MovementComponent, TransformComponent>().each())
		{
			m_Reticle = GameObject(entity, scene);
			m_ReticleObjectID = m_Reticle.GetGUID();
			m_BaseReticlePosZ = transform.GetPosition().z;
			m_ReticleBoostPosZ = m_BaseReticlePosZ + (m_BoostedFollowZ * -1.0f);
			m_ReticleBreakPosZ = m_BaseReticlePosZ + (m_BreakingFollowZ * 0.5f);
			break;
		}
	}

	void Player::BindFollowers(std::shared_ptr<Scene> scene, GameObject& hostObject)
	{
		// try to find a follower component that follows the reticle object
		// and the follower that follows the spline track itself
		// this is specific to this spline game set up where there is only one follow component that follows the reticle
		// going to use tags to filter out the track follower
		if (!m_Reticle)
			return; // no reticle object found. don't bother searching for follower

		entt::registry& registry = scene->GetRegistry();

		for (auto&& [entity, follower, idComponent, tagComponent] : registry.view<FollowComponent, IDComponent, TagComponent>().each())
		{
			if (follower.EntityToFollow == m_ReticleObjectID)
			{
				m_ReticleFollower = GameObject(entity, scene);
				m_ReticleFollowerID = idComponent.ID;
				m_BaseFollowZ = follower.Offset.z;
			}
			else if (tagComponent.Tag == m_TrackFollowerTag)
			{
				m_TrackFollowerID = idComponent.ID;
				m_TrackFollower = GameObject(entity, scene);
			}

		}

		if (m_ReticleFollower)
		{
			MovementComponent* followMover = m_ReticleFollower.TryGetComponent<MovementComponent>();
			if (followMover)
			{
				m_BaseReticleFollowerSpeed = followMover->Speed;
			}
		}

		if (m_TrackFollower)
		{
			MovementComponent* trackFollower = m_TrackFollower.TryGetComponent<MovementComponent>();
			if (trackFollower)
			{
				m_BaseTrackSpeed = trackFollower->Speed;
			}
		}
	}

	void Player::BindDeathCallback(std::shared_ptr<Scene> scene, GameObject& hostObject)
	{
		std::string callbackName = m_Name + std::to_string(hostObject.GetGUID());
		if (m_DamageableID != 0)
		{
			m_ObjectWithDamageable = scene->FindGameObjectByGUID(m_DamageableID);
			if (m_ObjectWithDamageable)
			{
				Damageable& damageable = m_ObjectWithDamageable.GetComponent<Damageable>();
				using std::placeholders::_1;

				damageable.AddOnDeathCallback(callbackName, std::bind(&Player::OnDeath, this, _1));
			}
		}
		else
		{
			// try to find a point collector in children or parent;
			Damageable* inParent = hostObject.GetComponentInParent<Damageable>();
			if (inParent)
			{
				m_ObjectWithDamageable = hostObject.GetParent();
				m_DamageableID = m_ObjectWithDamageable.GetGUID();
				using std::placeholders::_1;
				inParent->AddOnDeathCallback(callbackName, std::bind(&Player::OnDeath, this, _1));
			}
			else
			{
				std::vector<GameObject> inChildren = hostObject.GetChildrenWithComponent<PointCollector>();

				if (!inChildren.empty())
				{
					GameObject child = inChildren.front();
					Damageable& damageable = child.GetComponent<Damageable>();
					m_ObjectWithDamageable = child;

					m_DamageableID = child.GetGUID();

					using std::placeholders::_1;
					damageable.AddOnDeathCallback(callbackName, std::bind(&Player::OnDeath, this, _1));
				}
			}
		}
	}

	void Player::OnUpdate(GameObject& caller, float deltaTime)
	{
		caller.GetComponent<Player>().Update(caller, deltaTime);
	}
	void Player::OnCollisionStart(GameObject& caller, CollisionEvent& collision)
	{
		

	}
	void Player::OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime)
	{

	}

	void PlayerData::Draw()
	{
		ImGui::BeginDisabled();
		ImGui::Text("Score");
		ImGui::SameLine();
		ImGui::DragInt("##scoreValue", &Score);

		ImGui::Text("Enemies Defeated");
		ImGui::SameLine();
		ImGui::DragInt("##defeatedEnemies", &EnemiesDestroyed);

		ImGui::Text("Points Collected");
		ImGui::SameLine();
		ImGui::DragInt("##points", &PointsCollected);
		ImGui::EndDisabled();
	}
	void Player::StaticDraw(GameObject& selected)
	{
		Player& player = selected.GetComponent<Player>();
		player.Draw(selected);
	}
	void Player::Draw(GameObject& selected)
	{
		ImGuiPanel::CheckBox("Show Debug Score", &m_ShowPlayerDataInDetailsView);
		if (m_ShowPlayerDataInDetailsView)
		{
			m_Data.Draw();
		}

		ImGui::Separator();
		// point collector property ---------------------//
		std::string name = "";
		if (m_ObjectWithPointCollector)
			name = m_ObjectWithPointCollector.GetName();
		else if (m_PointCollectorID != 0)
		{
			std::shared_ptr<Scene> scene = selected.GetScene();
			GameObject collectorObject = scene->FindGameObjectByGUID(m_PointCollectorID);
			if (collectorObject)
			{
				m_ObjectWithPointCollector = collectorObject;
				name = m_ObjectWithPointCollector.GetName();
			}
		}

		ImGui::BeginDisabled();
		ImGuiPanel::DrawStringProperty("Point Collector", name, name);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject));
				GameObject droppedObject = *(const GameObject*)payload->Data;

				if (droppedObject.HasCompoenent<PointCollector>())
				{
					m_ObjectWithPointCollector = droppedObject;
					m_PointCollectorID = droppedObject.GetGUID();
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::BeginItemTooltip())
		{
			ImGui::Text("Drag object with a Point Collector Component from Heirarchy here to connect");
			ImGui::EndTooltip();
		}

		// Damageable property ---------------------//

		std::string damagableName = "";
		if (m_ObjectWithDamageable)
			damagableName = m_ObjectWithDamageable.GetName();
		else if (m_DamageableID != 0)
		{
			std::shared_ptr<Scene> scene = selected.GetScene();
			GameObject damageableObject = scene->FindGameObjectByGUID(m_DamageableID);
			if (damageableObject)
			{
				m_ObjectWithDamageable = damageableObject;
				damagableName = m_ObjectWithDamageable.GetName();
			}
		}

		ImGuiPanel::DrawStringProperty("Damageable", damagableName, damagableName);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject));
				GameObject droppedObject = *(const GameObject*)payload->Data;

				if (droppedObject.HasCompoenent<Damageable>())
				{
					m_ObjectWithDamageable = droppedObject;
					m_DamageableID = droppedObject.GetGUID();
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::BeginItemTooltip())
		{
			ImGui::Text("Drag object with a Damageable Component from Heirarchy here to connect");
			ImGui::EndTooltip();
		}

		ImGui::EndDisabled();

		// Movement stuff
		ImGui::SeparatorText("Boosting");
		ImGuiPanel::SliderParams params;
		params.ResetValue = 1.0f;
		params.Speed = 0.01f;
		params.Min = 0.0f;
		params.Max = 5.0f;
		
		// using inverse because editor side iteas easier to think of it as a distance behind the target in absolute value rather than signed.
		// this does mean negative values in editor will lead to trying to get ahead of the target
		float inverseBoostedOffset = -1.0f * m_BoostedFollowZ;
		if (ImGuiPanel::DrawFloatControl("Boosted Follow Depth", inverseBoostedOffset, params))
		{
			m_BoostedFollowZ = -1.0f * inverseBoostedOffset;
		}

		params.ResetValue = 2.0f;
		//speedParams.Min = 1.25f;
		ImGuiPanel::DrawFloatControl("Boost Speed Multiplier", m_BoostMultiplier, params);

		//ImGuiPanel::DrawFloatControl("Decay Time after Boost Released", m_BoostDecayTime, params);
		
		ImGui::SeparatorText("Breaking");
		// using inverse because editor side iteas easier to think of it as a distance behind the target in absolute value rather than signed.
		// this does mean negative values in editor will lead to trying to get ahead of the target
		params.ResetValue = 1.0f;
		float inverseBreakingOffset = -1.0f * m_BreakingFollowZ;
		if (ImGuiPanel::DrawFloatControl("Breaking Follow Depth", inverseBreakingOffset, params))
		{
			m_BreakingFollowZ = -1.0f * inverseBreakingOffset;
		}

		params.ResetValue = 0.25f;
		ImGuiPanel::DrawFloatControl("Break Speed Multiplier", m_BreakSpeedMultiplier, params);

}

	void Player::Serialize(StreamWriter* stream, const GameObject& sourceObject)
	{
		Player& player = sourceObject.GetComponent<Player>();
		stream->WriteObject<GUID>(player.m_PointCollectorID);
		stream->WriteObject<GUID>(player.m_DamageableID);
		stream->WriteObject<GUID>(player.m_ReticleObjectID);

		stream->WriteRaw<float>(player.m_BoostMultiplier);
		stream->WriteRaw<float>(player.m_BoostDecayTime);
		stream->WriteRaw<float>(player.m_BoostedFollowZ);

		stream->WriteRaw<float>(player.m_BreakSpeedMultiplier);
		stream->WriteRaw<float>(player.m_BreakDecayTime);
		stream->WriteRaw<float>(player.m_BreakingFollowZ);
	}
	bool Player::Deserialize(StreamReader* stream, GameObject& destinationObject)
	{
		Player& player = destinationObject.GetOrAddComponent<Player>();
		stream->ReadObject<GUID>(player.m_PointCollectorID);
		stream->ReadObject<GUID>(player.m_DamageableID);
		stream->ReadObject<GUID>(player.m_ReticleObjectID);

		stream->ReadRaw<float>(player.m_BoostMultiplier);
		stream->ReadRaw<float>(player.m_BoostDecayTime);
		stream->ReadRaw<float>(player.m_BoostedFollowZ);

		stream->ReadRaw<float>(player.m_BreakSpeedMultiplier);
		stream->ReadRaw<float>(player.m_BreakDecayTime);
		stream->ReadRaw<float>(player.m_BreakingFollowZ);
		return false;
	}

	void Player::OnLevelTrigger(int levelIndex)//std::filesystem::path nextLevelPath)
	{
		std::shared_ptr<ExampleGame> game = std::dynamic_pointer_cast<ExampleGame>( EngineContext::GetGame());
		if (game == nullptr)
		{
			Logger::Game(LOG_ERROR, "Aparently I cant cast example game down to its base game type and back up this way");
			return;
		}

		game->OnPlayerReachedEnd(m_Data, levelIndex);// nextLevelPath);
	}

	void Player::StartBoosting()
	{
		//m_AccumulatedBoostDecay = 0.0f;
		m_AccumulatedBoostTime = 0.0f;
		m_IsBoosting = true;
		m_IsBoostDecaying = false;
		SetNewSpeeds(m_BoostMultiplier);

		m_ReticleFollower.GetComponent<FollowComponent>().Offset.z = m_BoostedFollowZ;
	}

	void Player::StopBoosting()
	{
		m_AccumulatedBoostTime = 0.0f;
		m_IsBoosting = false;
		m_IsBoostDecaying = true;
		SetNewSpeeds(1.0f);
		m_ReticleFollower.GetComponent<FollowComponent>().Offset.z = m_BaseFollowZ;
	}

	void Player::StartBreaking()
	{
		m_AccumulatedBreakTime = 0.0f;
		//m_AccumulatedBreakDecay = 0.0f;
		m_IsBreaking = true;
		m_IsBreakDecaying = false;
		SetNewSpeeds(m_BreakSpeedMultiplier);
		m_ReticleFollower.GetComponent<FollowComponent>().Offset.z = m_BreakingFollowZ;
	}

	void Player::StopBreaking()
	{
		m_AccumulatedBreakTime = 0.0f;
		m_IsBreaking = false;
		m_IsBreakDecaying = true;
		SetNewSpeeds(1.0f);
		m_ReticleFollower.GetComponent<FollowComponent>().Offset.z = m_BaseFollowZ;
	}

	void Player::Update(GameObject& hostObject, float deltaTime)
	{
		
	}

	void Player::LerpReticle(float deltaTime)
	{
		//TODO Move playermovement out of game object and into a player class either this or another
	}

	void Player::OrientTowardsReticle(float deltaTime)
	{
		//TODO figure out why rotation to face a point is broken but rotate axis X degrees is working
	}

	

	void Player::SetNewSpeeds(float speedMult)
	{
		MovementComponent* trackMover = m_TrackFollower ? m_TrackFollower.TryGetComponent<MovementComponent>() : nullptr;
		MovementComponent* followMover = m_ReticleFollower ? m_ReticleFollower.TryGetComponent<MovementComponent>() : nullptr;

		if (trackMover)
		{
			trackMover->Speed = m_BaseTrackSpeed * speedMult;
		}
	}

	void Player::OnDeath(GUID killerID)
	{
		std::shared_ptr<ExampleGame> game = std::dynamic_pointer_cast<ExampleGame>(EngineContext::GetGame());
		if (game == nullptr)
		{
			Logger::Game(LOG_ERROR, "Aparently I cant cast example game down to its base game type and back up this way");
			return;
		}

		game->OnPlayerReachedEnd(m_Data);
	}


}