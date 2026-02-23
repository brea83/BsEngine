#include "TriggerNextScene.h"
#include "ScriptManager.h"
#include "Scene/GameObject.h"
#include "ImGui/ImGuiPanel.h"
#include "EngineContext.h"
namespace Pixie
{
	const std::string TriggerNextScene::m_Name = "Next Scene Trigger Volume";

	void TriggerNextScene::RegisterToScriptManager()
	{
		ScriptManager* scripts = ScriptManager::GetInstance();

		using std::placeholders::_1;
		using std::placeholders::_2;
		scripts->TryAddAttatchComponentFunction(m_Name, std::bind(&TriggerNextScene::AddMyComponentToGameObject, _1));
		scripts->TryAddRemoveComponentFunction(m_Name, std::bind(&TriggerNextScene::RemoveMyComponent, _1));

		scripts->TryAddOnUpdateFunction(m_Name, std::bind(&TriggerNextScene::OnUpdate, _1, _2));
		scripts->TryAddOnCollisionFunction(m_Name, std::bind(&TriggerNextScene::OnCollisionStart, _1, _2));

		scripts->TryAddDrawComponentFunction(m_Name, std::bind(&TriggerNextScene::Draw, _1));
	}

	void TriggerNextScene::AddMyComponentToGameObject(GameObject & hostObject)
	{
		if (hostObject.HasCompoenent<TriggerNextScene>())
			return;

		if (!hostObject.HasCompoenent<CollisionComponent>())
			hostObject.AddComponent<CollisionComponent>();

		hostObject.AddComponent<TriggerNextScene>();
	}

	void TriggerNextScene::RemoveMyComponent(GameObject& hostObject)
	{
		if (hostObject.HasCompoenent<TriggerNextScene>())
		{
			hostObject.RemoveComponent<TriggerNextScene>();
			return;
		}
	}

	void TriggerNextScene::OnUpdate(GameObject & caller, float deltaTime)
	{
		
	}

	void TriggerNextScene::OnCollisionStart(GameObject & caller, CollisionEvent & collision)
	{
		TriggerNextScene& trigger = caller.GetComponent<TriggerNextScene>();
		if (trigger.triggered == true)
			return;

		GUID myId = caller.GetGUID();
		Entity other = Entity();
		if (collision.A.GetComponent<IDComponent>().ID == myId)
			other = collision.B;
		else
			other = collision.A;

		if (other.TryGetComponent<TagComponent>() == nullptr)
			return;

		if (other.GetComponent<TagComponent>().Tag != trigger.m_TagThatActivatesTrigger)
			return;

		Logger::Core(LOG_DEBUG, "Trigger next scene trigger on object {}, collided with player tag on object {}", caller.GetName(), other.GetName());
		Logger::Core(LOG_DEBUG, "Trigger needs to search for and trigger load for scene named: {}", trigger.m_NextSceneName);

		EngineContext::GetEngine()->RequestSceneChange(trigger.m_NextSceneName);

		trigger.triggered = true;
	}

	void TriggerNextScene::OnCollisionOngoing(GameObject & caller, CollisionEvent & collision, float deltaTime)
	{}

	void TriggerNextScene::Draw(GameObject & selected)
	{
		if (!selected.HasCompoenent<TriggerNextScene>())
			return;
		TriggerNextScene& trigger = selected.GetComponent<TriggerNextScene>();
		
		if (ImGuiPanel::FileProperty("Next Scene", trigger.m_NextSceneName, "Pixie Game Settings (*.pixieIni)\0*.pixieIni\0"))
		{
			//std::filesystem::path path = trigger.m_NextSceneName;
		}
	}
}