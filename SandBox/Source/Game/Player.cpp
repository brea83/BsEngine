#include "Player.h"
#include "Scene/GameObject.h"
#include "Physics/PhysicsEngine.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"

#include "CombatComponents.h"
#include "CollisionBehavior/PointCollector.h"

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

		if (player.m_PointCollectorID != 0)
		{
			player.m_ObjectWithPointCollector = scene->FindGameObjectByGUID(player.m_PointCollectorID);
			if (player.m_ObjectWithPointCollector)
			{
				PointCollector& collector = player.m_ObjectWithPointCollector.GetComponent<PointCollector>();
				using std::placeholders::_1;
				collector.SetOnPointsCollectedCallback(std::bind(&Player::CollectPoints, &player, _1));
			}
		}
		else
		{
			// try to find a point collector in children or parent;
			PointCollector* inParent = hostObject.GetComponentInParent<PointCollector>();
			if (inParent)
			{
				player.m_ObjectWithPointCollector = scene->FindGameObjectByGUID(inParent->ID);
				player.m_PointCollectorID = inParent->ID;
				using std::placeholders::_1;
				inParent->SetOnPointsCollectedCallback(std::bind(&Player::CollectPoints, &player, _1));
			}
			else
			{
				std::vector<GameObject> inChildren = hostObject.GetChildrenWithComponent<PointCollector>();

				if (!inChildren.empty())
				{
					GameObject child = inChildren.front();
					PointCollector& collector = child.GetComponent<PointCollector>();
					player.m_ObjectWithPointCollector = child;

					player.m_PointCollectorID = child.GetGUID();

					using std::placeholders::_1;
					collector.SetOnPointsCollectedCallback(std::bind(&Player::CollectPoints, &player, _1));
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

		ImGui::EndDisabled();
	}

	void Player::Serialize(StreamWriter* stream, const GameObject& sourceObject)
	{
		Player& player = sourceObject.GetComponent<Player>();
		stream->WriteObject<GUID>(player.m_PointCollectorID);
	}
	bool Player::Deserialize(StreamReader* stream, GameObject& destinationObject)
	{
		Player& player = destinationObject.GetOrAddComponent<Player>();
		stream->ReadObject<GUID>(player.m_PointCollectorID);
		return false;
	}

}