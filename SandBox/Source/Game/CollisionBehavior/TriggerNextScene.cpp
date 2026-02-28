#include "TriggerNextScene.h"
#include "ScriptManager.h"
#include "Pixie.h"
#include "ImGui/ImGuiPanel.h"
//#include "EngineContext.h"
#include "../ExampleGame.h"

#include "../Player.h"

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
		newScript.OnCollisionOngoing = [](GameObject& object, CollisionEvent& event, float time) {};

		scripts->TryStoreScript(m_Name, newScript);

	}

	void TriggerNextScene::AddMyComponentToGameObject(GameObject & hostObject)
	{
		if (hostObject.HasCompoenent<TriggerNextScene>())
			return;

		if (!hostObject.HasCompoenent<CollisionComponent>())
			hostObject.AddComponent<CollisionComponent>();

		TriggerNextScene& trigger = hostObject.AddComponent<TriggerNextScene>();
		trigger.m_Game = std::dynamic_pointer_cast<ExampleGame>(EngineContext::GetGame());
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

		entt::registry& registry = caller.GetScene()->GetRegistry();

		for (auto&& [entity, player] : registry.view<Player>().each())
		{
			// only supporting single player right now only act on first player found
			player.OnLevelTrigger(trigger.m_NextLevelIndex);
			break;
		}

		trigger.triggered = true;
	}

	void TriggerNextScene::OnCollisionOngoing(GameObject & caller, CollisionEvent & collision, float deltaTime)
	{}

	void TriggerNextScene::Draw(GameObject & selected)
	{
		if (!selected.HasCompoenent<TriggerNextScene>())
			return;
		TriggerNextScene& trigger = selected.GetComponent<TriggerNextScene>();
		
		//
		//if (ImGuiPanel::FileProperty("Next Scene", trigger.m_NextSceneName, "Pixie Game Settings (*.pixie)\0*.pixie\0"))
		//{
		//	//std::filesystem::path path = trigger.m_NextSceneName;
		//}

		//---------- new method
		trigger.DrawLevelSelect(selected);
	}

	void Pixie::TriggerNextScene::DrawLevelSelect(GameObject& selected)
	{
		if (m_Game == nullptr)
			return;

		auto& levels = m_Game->GetAllLevelData();

		auto& scenes = m_Game->GetScenePaths();

		std::vector<std::string> levelNames;

		for (auto& pair : levels)
		{
			if (pair.first == 0)
			{
				levelNames.push_back("Select Level");
				continue;
			}
			int sceneIndex = pair.second.FilePathIndex;
			bool bSceneValid = sceneIndex < scenes.size() && sceneIndex >= 0;

			if (bSceneValid)
			{
				levelNames.push_back("Level " + std::to_string(pair.first) + ": " + scenes[sceneIndex].filename().string());
			}
			else
			{
				levelNames.push_back("Level " + std::to_string(pair.first) + ": scene invalid");
			}
		}

		levelNames.push_back("END GAME");

		static int selectedLevel = 0;
		std::string previewValue;
		if (selectedLevel < 1)
			previewValue = "select level";
		else if (selectedLevel == levels.size())
		{
			previewValue = "END GAME: NO LEVEL";
		}
		else
		{
			previewValue = levelNames[selectedLevel];
		}
		

		if (ImGui::BeginCombo("##levelPicker", previewValue.c_str()))
		{
			for (int i = 0; i < levelNames.size(); i++)
			{
				const bool is_selected = (selectedLevel == i);
				if (ImGui::Selectable(levelNames[i].c_str(), is_selected))
				{
					selectedLevel = i;

					if (selectedLevel < levels.size() && selectedLevel > 0)
					{
						m_NextLevelIndex = selectedLevel;
					}
					else
					{
						m_NextLevelIndex = levels.size();
					}
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selectedLevel != 0)
		{
			std::string feedback = "Selected " + levelNames[selectedLevel];
			ImGui::TextWrapped(feedback.c_str());
		}
		else
		{
			ImGui::TextWrapped("No Level selected, trigger will act as an END GAME trigger");
		}
		
		
		
	}

	void TriggerNextScene::Serialize(StreamWriter* stream, const GameObject& sourceObject)
	{
		TriggerNextScene& component = sourceObject.GetComponent<TriggerNextScene>();

		stream->WriteString(m_Name);
		//stream->WriteString(component.m_NextSceneName);
		stream->WriteRaw<int>(component.m_NextLevelIndex);
		stream->WriteString(component.m_TagThatActivatesTrigger);
	}
	bool TriggerNextScene::Deserialize(StreamReader * stream, GameObject& destinationObject)
	{
		TriggerNextScene& component = destinationObject.GetComponent<TriggerNextScene>();

		std::string name;
		stream->ReadString(name);
		if (name != m_Name)
			return false;

		//stream->ReadString(component.m_NextSceneName);
		stream->ReadRaw<int>(component.m_NextLevelIndex);
		stream->ReadString(component.m_TagThatActivatesTrigger);
		return true;
	}
}