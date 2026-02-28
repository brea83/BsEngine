#include "CombatComponents.h"
#include "ImGui/ImGuiPanel.h"
#include "Scene/GameObject.h"
#include "CollisionBehavior/Damageable.h"
#include "Player/Player.h"


// Attack Component --------------
const std::string Attack::m_Name = "Attack Component";
void Attack::RegisterToScriptManager()
{
	Pixie::ScriptManager* scripts = Pixie::ScriptManager::GetInstance();

	using std::placeholders::_1;
	using std::placeholders::_2;
	using std::placeholders::_3;
	Pixie::StoredScript newScript;
	newScript.AttachComponent = std::bind(&Attack::Attach, _1);
	newScript.CopyComponent = std::bind(&Attack::Copy, _1, _2);
	newScript.RemoveComponent = std::bind(&Attack::Remove, _1);
	newScript.Serialize = std::bind(&Attack::Serialize, _1, _2);
	newScript.Deserialize = std::bind(&Attack::Deserialize, _1, _2);
	newScript.Draw = std::bind(&Attack::Draw, _1);


	scripts->TryStoreScript(m_Name, newScript);
}

void Attack::Draw(Pixie::GameObject& selected)
{
	if (!selected.HasCompoenent<Attack>())
		return;
	Attack& component = selected.GetComponent<Attack>();
	ImGui::Text("Damage");
	ImGui::SameLine();
	ImGui::DragInt("##maxHealth", &component.Damage, 1.0f);
}

void Attack::Attach(Pixie::GameObject& hostObject)
{
	hostObject.AddComponent<Attack>();
}

void Attack::Remove(Pixie::GameObject& selected)
{
	selected.RemoveComponent<Attack>();
}

void Attack::Copy(Pixie::GameObject& sourceObject, Pixie::GameObject& destinationObject)
{
	if (!sourceObject.HasCompoenent<Attack>())
		return;
	Attack& source = sourceObject.GetComponent<Attack>();

	destinationObject.AddOrReplaceComponent<Attack>(source);
}

void Attack::Serialize(Pixie::StreamWriter* stream, const Pixie::GameObject& sourceObject)
{
	if (!sourceObject.HasCompoenent<Attack>())
		return;
	Attack& component = sourceObject.GetComponent<Attack>();
	stream->WriteRaw<int>(component.Damage);
}
bool Attack::Deserialize(Pixie::StreamReader* stream, Pixie::GameObject& destinationObject)
{
	Attack& component = destinationObject.GetOrAddComponent<Attack>();
	stream->ReadRaw<int>(component.Damage);
	return true;
}

// Point Scoring component ------------

const std::string ScoresPoints::m_Name = "Points Containing Component";

void ScoresPoints::RegisterToScriptManager()
{
	Pixie::ScriptManager* scripts = Pixie::ScriptManager::GetInstance();

	using std::placeholders::_1;
	using std::placeholders::_2;
	using std::placeholders::_3;
	Pixie::StoredScript newScript;
	newScript.AttachComponent = std::bind(&ScoresPoints::Attach, _1);
	newScript.CopyComponent = std::bind(&ScoresPoints::Copy, _1, _2);
	newScript.RemoveComponent = std::bind(&ScoresPoints::Remove, _1);

	newScript.Serialize = std::bind(&ScoresPoints::Serialize, _1, _2);
	newScript.Deserialize = std::bind(&ScoresPoints::Deserialize, _1, _2);
	newScript.Draw = std::bind(&ScoresPoints::Draw, _1);

	newScript.OnBeginPlay = std::bind(&ScoresPoints::OnBeginPlay, _1);

	scripts->TryStoreScript(m_Name, newScript);
}

void ScoresPoints::Attach(Pixie::GameObject& hostObject)
{
	ScoresPoints& pointsComponent = hostObject.AddComponent<ScoresPoints>();
	pointsComponent.m_OwnId = hostObject.GetGUID();
}

void ScoresPoints::Draw(Pixie::GameObject& selected)
{
	if (!selected.HasCompoenent<ScoresPoints>())
		return;
	ScoresPoints& component = selected.GetComponent<ScoresPoints>();

	if (selected.HasCompoenent<Pixie::Damageable>())
	{
		ImGui::Text("Points Awarded On Death");
		ImGui::SameLine();
		ImGui::Checkbox("##pointsOnDestroy", &component.BPointsDeliveredOnDeath);

		ImGui::Text("Points Awarded Player Collision");
		ImGui::SameLine();
		ImGui::Checkbox("##pointsOnCollision", &component.BPointsDeliveredOnCollision);
	}
	else
	{
		ImGui::TextWrapped("Points are Awarded on Player Collision, add damageable script to allow points on death");
	}

	ImGui::Text("Points");
	ImGui::SameLine();
	ImGui::DragInt("##PointsToAward", &component.Value, 1.0f);
}

void ScoresPoints::Remove(Pixie::GameObject& selected)
{
	selected.RemoveComponent<ScoresPoints>();
}

void ScoresPoints::Copy(Pixie::GameObject& sourceObject, Pixie::GameObject& destinationObject)
{
	if (!sourceObject.HasCompoenent<ScoresPoints>())
		return;
	ScoresPoints& source = sourceObject.GetComponent<ScoresPoints>();

	destinationObject.AddOrReplaceComponent<ScoresPoints>(source);
}

void ScoresPoints::Serialize(Pixie::StreamWriter* stream, const Pixie::GameObject& sourceObject)
{
	if (!sourceObject.HasCompoenent<ScoresPoints>())
		return;
	ScoresPoints& component = sourceObject.GetComponent<ScoresPoints>();
	stream->WriteRaw<int>(component.Value);
}
bool ScoresPoints::Deserialize(Pixie::StreamReader* stream, Pixie::GameObject& destinationObject)
{
	ScoresPoints& component = destinationObject.GetOrAddComponent<ScoresPoints>();
	stream->ReadRaw<int>(component.Value);
	return true;
}

void ScoresPoints::OnBeginPlay(Pixie::GameObject& hostObject)
{
	if ( !hostObject.HasCompoenent<ScoresPoints>())
		return;

	ScoresPoints& pointsComponent = hostObject.GetComponent<ScoresPoints>();//.BindOnDeathCallback(hostObject);
	pointsComponent.m_CurrentScene = hostObject.GetScene();

	if (pointsComponent.BPointsDeliveredOnDeath && hostObject.HasCompoenent<Pixie::Damageable>())
	{
		Pixie::Damageable& damageable = hostObject.GetComponent<Pixie::Damageable>();
		Pixie::GUID id = hostObject.GetGUID();

		std::string callbackName = m_Name + std::to_string(id);

		using std::placeholders::_1;
		std::function<void(Pixie::GUID)> callback = [&pointsComponent](Pixie::GUID killerGuid) { pointsComponent.OnDeath(killerGuid); };
	
		damageable.AddOnDeathCallback(callbackName, callback);
	}

	if (!hostObject.HasCompoenent<Pixie::CollisionComponent>())
		Pixie::Logger::Game(LOG_WARNING, "{}, has a points component but no collider for the player to find them with", hostObject.GetName());

}

void ScoresPoints::OnDeath(Pixie::GUID& killerGuid)
{
	if (m_CurrentScene == nullptr)
		return;

	Pixie::GameObject self = m_CurrentScene->FindGameObjectByGUID(m_OwnId);
	Pixie::GameObject killerObject = m_CurrentScene->FindGameObjectByGUID(killerGuid);

	if (!killerObject.HasCompoenent<Pixie::TagComponent>())
		return;

	Pixie::TagComponent& tag = killerObject.GetComponent<Pixie::TagComponent>();
	if (tag.Tag != "Player" && tag.Tag != "player" && tag.Tag != "PLAYER")
		return;

	entt::registry& registry = killerObject.GetScene()->GetRegistry();

	for (auto&& [entity, player] : registry.view<Pixie::Player>().each())
	{
		// only supporting single player right now only act on first player found
		player.PointsOnKill(Value);
	}
}
