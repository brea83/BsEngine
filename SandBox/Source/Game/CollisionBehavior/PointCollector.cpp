#include "PointCollector.h"
#include "Scene/GameObject.h"
#include "Physics/PhysicsEngine.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"
#include "../CombatComponents.h"

namespace Pixie
{
	const std::string PointCollector::m_Name = "PointCollector Component";

	void PointCollector::RegisterToScriptManager()
	{
		ScriptManager* scripts = ScriptManager::GetInstance();

		using std::placeholders::_1;
		using std::placeholders::_2;
		//using std::placeholders::_3;
		StoredScript newScript;
		newScript.AttachComponent = std::bind(&PointCollector::Attach, _1);
		newScript.CopyComponent = std::bind(&PointCollector::Copy, _1, _2);
		newScript.RemoveComponent = std::bind(&PointCollector::Remove, _1);

		/*newScript.Serialize = std::bind(&PointCollector::Serialize, _1, _2);
		newScript.Deserialize = std::bind(&PointCollector::Deserialize, _1, _2);*/

		newScript.OnCollision = std::bind(&PointCollector::OnCollisionStart, _1, _2);
		newScript.Draw = std::bind(&PointCollector::Draw, _1);


		scripts->TryStoreScript(m_Name, newScript);
	}

	void PointCollector::Attach(GameObject& hostObject)
	{
		PointCollector& pointCollector = hostObject.AddComponent<PointCollector>();

		pointCollector.ID = hostObject.GetGUID();
		
		if (!hostObject.HasCompoenent<CollisionComponent>())
		{
			CollisionComponent& collision = hostObject.AddComponent<CollisionComponent>();
		}
	}
	void PointCollector::Remove(GameObject& hostObject)
	{
		hostObject.RemoveComponent<PointCollector>();
	}
	void PointCollector::Copy(GameObject& sourceObject, GameObject& destinationObject)
	{
		if (!sourceObject.HasCompoenent<PointCollector>())
			return;
		PointCollector& source = sourceObject.GetComponent<PointCollector>();

		destinationObject.AddOrReplaceComponent<PointCollector>(source);
	}

	void PointCollector::OnUpdate(GameObject& caller, float deltaTime)
	{
		// TODO animate visualizer
	}
	void PointCollector::OnCollisionStart(GameObject& caller, CollisionEvent& collision)
	{
		PointCollector& pointsCollector = caller.GetComponent<PointCollector>();


		Entity other = collision.ExtractOtherObject(caller);

		if (other.HasCompoenent< ScoresPoints>())
		{
			ScoresPoints& points = other.GetComponent<ScoresPoints>();
			if (points.BPointsDeliveredOnCollision && pointsCollector.m_OnPointsCollectedCallback)
			{
				pointsCollector.m_OnPointsCollectedCallback(points.Value);
			}
		}

	}
	
	void PointCollector::Draw(GameObject& selected)
	{
		PointCollector& pointsCollector = selected.GetComponent<PointCollector>();
		ImGui::BeginDisabled();

		ImGui::TextWrapped("Collects points when the collider on this same object collides with an object that has both a collider and a Scores Points component");

		ImGui::EndDisabled();
	}


}