#pragma once
#include "Source/Game.h"
#include "Scene/Player/PlayerInput.h"
#include "Player.h"

namespace Pixie
{
	class ExampleGame : public Game
	{
	public:
		ExampleGame() { OnCreate(); }
		~ExampleGame();

		// check FilePathIndex for -1 to see if struct is unitiialized
		struct Level
		{
			int FilePathIndex{ -1 }; // lookup in m_ScenePaths
			PlayerData Scores {};
			bool IsFinalLevel{ false };
		};
		
		// Inherited via Game
		void OnCreate() override;
		void OnBeginPlay(std::shared_ptr<Scene> scene);
		void OnBeginPlay() override;
		void OnUpdate(float deltaTime) override;

		virtual bool OnEvent(Event& event) override;

		void Pause() override;
		void UnPause() override;
		void SetState(const std::string_view& stateType) override;
		//GameState* GetCurrentState() override;
		//GameState* GetPreviousState() override;

		// collects player data from player on level end. 
		// if player dies dont send next level path
		void OnLevelEnd(const PlayerData& data, std::filesystem::path nextLevelPath = "");

		// returns id == 0 if none found
		virtual GUID GetPlayerID(int index = 0) override;
		// check Score for -1 to see if struct is unitiialized
		PlayerData GetCurrentPlayerData();
		
		std::unordered_map<int, Level>& GetAllLevelData() { return m_LevelData; }

	private:

		int m_CurrentLevel{ 0 };

		PlayerInputSystem* m_InputSystem{ nullptr };

		// int is shorthand for level name lvl 1 lvl 2 etc
		std::unordered_map<int, Level> m_LevelData;

		void FindAllPlayers();

		bool OnSceneChangedEvent(SceneChangedEvent& event);
		bool OnGameStateRequest(GameStateChangeRequestEvent& event);
	};
}