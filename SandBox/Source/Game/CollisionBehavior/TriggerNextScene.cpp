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
		StoredScript newScript;
		newScript.AttachComponent = std::bind(&TriggerNextScene::AddMyComponentToGameObject, _1);
		newScript.CopyComponent = std::bind(&TriggerNextScene::CopyComponent, _1, _2);
		newScript.RemoveComponent = std::bind(&TriggerNextScene::RemoveMyComponent, _1);
		newScript.Serialize = std::bind(&TriggerNextScene::Serialize, _1, _2);
		newScript.Deserialize = std::bind(&TriggerNextScene::Deserialize, _1, _2);
		newScript.Draw = std::bind(&TriggerNextScene::Draw, _1);
		newScript.OnUpdate = std::bind(&TriggerNextScene::OnUpdate, _1, _2);
		newScript.OnCollision = std::bind(&TriggerNextScene::OnCollisionStart, _1, _2);
		newScript.OnCollisionOngoing = [](GameObject& object, CollisionEvent& event) {};

		scripts->TryStoreScript(m_Name, newScript);

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

	void TriggerNextScene::CopyComponent(GameObject& sourceObject, GameObject& destinationObject)
	{
		if (!sourceObject.HasCompoenent<TriggerNextScene>())
			return;
		TriggerNextScene& source = sourceObject.GetComponent<TriggerNextScene>();

		destinationObject.AddOrReplaceComponent<TriggerNextScene>(source);

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

		//Logger::Core(LOG_DEBUG, "Trigger next scene trigger on object {}, collided with player tag on object {}", caller.GetName(), other.GetName());
		//Logger::Core(LOG_DEBUG, "Trigger needs to search for and trigger load for scene named: {}", trigger.m_NextSceneName);

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
		
		if (ImGuiPanel::FileProperty("Next Scene", trigger.m_NextSceneName, "Pixie Game Settings (*.pixie)\0*.pixie\0"))
		{
			//std::filesystem::path path = trigger.m_NextSceneName;
		}
	}
	void TriggerNextScene::Serialize(StreamWriter* stream, const GameObject& sourceObject)
	{
		TriggerNextScene& component = sourceObject.GetComponent<TriggerNextScene>();

		stream->WriteString(m_Name);
		stream->WriteString(component.m_NextSceneName);
		stream->WriteString(component.m_TagThatActivatesTrigger);
	}
	bool TriggerNextScene::Deserialize(StreamReader * stream, GameObject& sourceObject)
	{
		TriggerNextScene& component = sourceObject.GetComponent<TriggerNextScene>();

		std::string name;
		stream->ReadString(name);
		if (name != m_Name)
			return false;

		stream->ReadString(component.m_NextSceneName);
		stream->ReadString(component.m_TagThatActivatesTrigger);
		return true;
	}
}