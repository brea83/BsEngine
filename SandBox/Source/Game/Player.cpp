#include "Player.h"
#include "Scene/GameObject.h"
#include "Physics/PhysicsEngine.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"
#include "EngineContext.h"

#include "ExampleGame.h"
#include "CombatComponents.h"
#include "CollisionBehavior/PointCollector.h"
#include "CollisionBehavior/Damageable.h"

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
		newScript.Draw = std::bind(&Player::Draw, _1);

		newScript.OnBeginPlay = std::bind(&Player::OnBeginPlay, _1);

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

	}
	void Player::OnCollisionStart(GameObject& caller, CollisionEvent& collision)
	{
		

	}
	void Player::OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime)
	{

	}
	void Player::Draw(GameObject& selected)
	{
		Player& player = selected.GetComponent<Player>();
		ImGui::BeginDisabled();

		ImGui::Text("Score");
		ImGui::SameLine();
		ImGui::DragInt("##scoreValue", &player.m_Data.Score);

		ImGui::Text("Enemies Defeated");
		ImGui::SameLine();
		ImGui::DragInt("##defeatedEnemies", &player.m_Data.EnemiesDestroyed);

		ImGui::Text("Points Collected");
		ImGui::SameLine();
		ImGui::DragInt("##points", &player.m_Data.PointsCollected);

		ImGui::Separator();
		// point collector property ---------------------//
		std::string name = "";
		if (player.m_ObjectWithPointCollector)
			name = player.m_ObjectWithPointCollector.GetName();
		else if (player.m_PointCollectorID != 0)
		{
			std::shared_ptr<Scene> scene = selected.GetScene();
			GameObject collectorObject = scene->FindGameObjectByGUID(player.m_PointCollectorID);
			if (collectorObject)
			{
				player.m_ObjectWithPointCollector = collectorObject;
				name = player.m_ObjectWithPointCollector.GetName();
			}
		}

		ImGuiPanel::DrawStringProperty("Point Collector", name, name);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HEIRARCHY_ITEM"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject));
				GameObject droppedObject = *(const GameObject*)payload->Data;

				if (droppedObject.HasCompoenent<PointCollector>())
				{
					player.m_ObjectWithPointCollector = droppedObject;
					player.m_PointCollectorID = droppedObject.GetGUID();
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::BeginItemTooltip())
		{
			ImGui::Text("Drag object with a Damageable Component from Heirarchy here to connect");
			ImGui::EndTooltip();
		}

		// Damageable property ---------------------//

		std::string damagableName = "";
		if (player.m_ObjectWithDamageable)
			damagableName = player.m_ObjectWithDamageable.GetName();
		else if (player.m_DamageableID != 0)
		{
			std::shared_ptr<Scene> scene = selected.GetScene();
			GameObject damageableObject = scene->FindGameObjectByGUID(player.m_DamageableID);
			if (damageableObject)
			{
				player.m_ObjectWithDamageable = damageableObject;
				damagableName = player.m_ObjectWithDamageable.GetName();
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
					player.m_ObjectWithDamageable = droppedObject;
					player.m_DamageableID = droppedObject.GetGUID();
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
	}

	void Player::Serialize(StreamWriter* stream, const GameObject& sourceObject)
	{
		Player& player = sourceObject.GetComponent<Player>();
		stream->WriteObject<GUID>(player.m_PointCollectorID);
		stream->WriteObject<GUID>(player.m_DamageableID);
		stream->WriteObject<GUID>(player.m_AttackID);
	}
	bool Player::Deserialize(StreamReader* stream, GameObject& destinationObject)
	{
		Player& player = destinationObject.GetOrAddComponent<Player>();
		stream->ReadObject<GUID>(player.m_PointCollectorID);
		stream->ReadObject<GUID>(player.m_DamageableID);
		stream->ReadObject<GUID>(player.m_AttackID);
		return false;
	}

	void Player::OnLevelTrigger(std::filesystem::path nextLevelPath)
	{
		std::shared_ptr<ExampleGame> game = std::dynamic_pointer_cast<ExampleGame>( EngineContext::GetGame());
		if (game == nullptr)
		{
			Logger::Game(LOG_ERROR, "Aparently I cant cast example game down to its base game type and back up this way");
			return;
		}

		game->OnPlayerReachedEnd(m_Data, nextLevelPath);
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