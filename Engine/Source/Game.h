#pragma once
#include "GameState.h"
#include "Scene/GameObject.h"

namespace Pixie
{

	// persistent states state machine, keeps one copy of each state and switches between them
	class GameStateMachine
	{
	public:
		GameStateMachine() = default;
		GameStateMachine(std::unordered_map<std::string_view, GameState*> states)
			: m_States(states) { }

		void SwitchState(const std::string_view& stateType);
		

		void UpdateState(float deltaTime);
		void OnImGuiRender();

		GameState* GetCurrentState() { return m_CurrentState; }
		GameState* GetPreviousState() { return m_CurrentState; }

		GameState* GetStateByType(const std::string_view& stateType);
	private:
		std::unordered_map<std::string_view, GameState*> m_States;
		GameState* m_CurrentState{ nullptr };
		GameState* m_PreviousState{ nullptr };

		//todo add event that dispatches on state changes
	};
	

	class Game
	{
	public:
		Game() = default;
		virtual ~Game() { }


		//returns -1 if index does not exist in player array
		virtual uint64_t GetPlayerID(size_t playerIndex);
		// will return a entt::null scene == nullptr game object if player guid not found
		virtual GameObject GetPlayerByGUID(uint64_t guid);
		// will return a entt::null scene == nullptr game object if player not found
		virtual GameObject GetPlayer(size_t playerIndex);

		// adds player to first available spot in player array m_Players
		virtual void AddPlayer(uint64_t guid);
		

		virtual void OnCreate() { };
		virtual void OnBeginPlay() {};
		virtual void OnUpdate(float deltaTime) {};

		virtual bool OnEvent(Event& event) { return false; }

		virtual void OnImGuiRender() { m_GameStateMachine.OnImGuiRender(); }

		virtual void Pause() {};
		virtual void UnPause() {};

		virtual void SetState(const std::string_view& stateType) {};

		virtual GameState* GetCurrentState() { return m_GameStateMachine.GetCurrentState(); }
		virtual GameState* GetPreviousState() { return m_GameStateMachine.GetPreviousState(); }

		virtual const std::vector<std::filesystem::path>& GetScenePaths() const { return m_ScenePaths; }
		virtual std::vector<std::filesystem::path>& GetScenePathsEditable() { return m_ScenePaths; }
		virtual void AddScenePath( std::filesystem::path path);
		virtual void ReplaceScenePath(int index, std::filesystem::path path);
		virtual void RemoveScene(int index);

		virtual void SaveSettings(std::filesystem::path filePath);
		virtual void LoadSettings(std::filesystem::path filePath);

		virtual GUID GetPlayerID(int index = 0) { return GUID(0); };

		// only use durring the drawImGui phase. needs the imgui frame start and end before and after it
		virtual void DrawEditor();
	protected:
		std::string m_Title{ "DefaultTitle" };
		std::filesystem::path m_SettingsPath{ "" };
		GameStateMachine m_GameStateMachine;
		std::vector<GUID> m_Players;

		std::shared_ptr<Scene> m_CurrentScene{ nullptr };


		// maybe replace this with a scene manager?
		/*std::unordered_map<std::string, std::filesystem::path>*/std::vector<std::filesystem::path> m_ScenePaths;
	};
}