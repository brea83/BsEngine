#include "BsPrecompileHeader.h"
#include "IComponentScript.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"

const std::string Pixie::TestComponentScript::m_Name = "Test Component Script";

void Pixie::TestComponentScript::RegisterToScriptManager() const
{
	ScriptManager* scripts = ScriptManager::GetInstance();

	using std::placeholders::_1;
	scripts->TryAddCombinedScriptAdd(m_Name, std::bind(&TestComponentScript::AddMyComponentToGameObject, _1));
	//using std::placeholders::_2;
	//scripts->TryAddOnUpdateFunction(m_Name, std::bind(&TestComponentScript::OnUpdate, std::placeholders::_2))
}

void Pixie::TestComponentScript::AddMyComponentToGameObject(GameObject& hostObject)
{
	hostObject.AddComponent<TestComponentScript>();
}

void Pixie::TestComponentScript::OnUpdate(GameObject& caller, float deltaTime)
{
	TestComponentScript component = caller.GetComponent<TestComponentScript>();
	if (component.m_UpdateCount > component.m_MaxUpdates)
		return;

	if (component.m_UpdateCount == component.m_MaxUpdates)
	{
		Logger::Core(LOG_DEBUG, "{} updates reached", component.m_MaxUpdates);
		component.m_UpdateCount++;
		return;
	}

	if (component.m_UpdateCount < component.m_MaxUpdates)
	{
		Logger::Core(LOG_DEBUG, "{} updates", component.m_UpdateCount);
		component.m_UpdateCount++;
		return;
	}
}

void Pixie::TestComponentScript::OnCollisionStart(GameObject & caller, CollisionEvent & collision)
{
	TestComponentScript component = caller.GetComponent<TestComponentScript>();
	if (component.m_CollisionCount > component.m_MaxCollisions)
	{
		Logger::Core(LOG_DEBUG, "Ignoring this collision and Resetting Collision count");
		component.m_CollisionCount = 0;
		return;
	}

	if (component.m_CollisionCount == component.m_MaxCollisions)
	{
		Logger::Core(LOG_DEBUG, "{}, collision max reached", component.m_MaxCollisions);
		component.m_CollisionCount++;
		return;
	}

	if (component.m_CollisionCount < component.m_MaxCollisions)
	{
		Logger::Core(LOG_DEBUG, "{} collisions", component.m_CollisionCount);
		component.m_CollisionCount++;
		return;
	}
}

void Pixie::TestComponentScript::OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime)
{
	TestComponentScript component = caller.GetComponent<TestComponentScript>();
	if (component.m_TimeInCollision < FLT_MAX)
	{
		component.m_TimeInCollision += deltaTime;
		Logger::Core(LOG_DEBUG, "{} time spent in collisions", component.m_TimeInCollision);
		return;
	}
}

void Pixie::TestComponentScript::DrawComponent(GameObject & selected)
{
	TestComponentScript component = selected.GetComponent<TestComponentScript>();
	ImGui::TextWrapped("Test script, counts some update calls. counts collisions, and time spend in collisions");

	ImGui::Text("Max Updates to Count");
	ImGui::SameLine();
	ImGui::InputInt("##dontshowlabel", &component.m_MaxUpdates);

	ImGui::Text("Max Updates to Count");
	ImGui::SameLine();
	ImGui::InputInt("##dontshowlabel", &component.m_MaxCollisions);
}
