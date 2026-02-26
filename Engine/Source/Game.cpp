#include "BsPrecompileHeader.h"
#include "Game.h"
#include "EngineContext.h"
#include "Resources/FileStream.h"

namespace Pixie
{
	//==========================
	// Game Base Class
	//==========================

	uint64_t Game::GetPlayerID(size_t playerIndex)
	{
		if (m_Players.size() > playerIndex)
			return m_Players[playerIndex];
		else
			return -1;
	}

	GameObject Game::GetPlayerByGUID(uint64_t guid)
	{
		if (std::find(m_Players.begin(), m_Players.end(), guid) != m_Players.end())
		{
			std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();
			return scene->FindGameObjectByGUID(guid);
		}
		else
			return GameObject();
	}

	GameObject Game::GetPlayer(size_t playerIndex)
	{
		if (m_Players.size() > playerIndex)
			return GetPlayerByGUID(m_Players[playerIndex]);
		else
			return GameObject();
	}

	void Game::AddPlayer(uint64_t guid)
	{
		if (std::find(m_Players.begin(), m_Players.end(), guid) == m_Players.end())
		{
			m_Players.push_back(guid);
			return;
		}

		Logger::Core(LOG_WARNING, "Tried to add player (guid: {}), but they had already been added", std::to_string(guid));
	}

	void Game::AddScenePath( std::filesystem::path path)
	{
		m_ScenePaths.push_back(path);
	}

	void Game::ReplaceScenePath(int index, std::filesystem::path path)
	{
		if (index < 0 || index >= m_ScenePaths.size())
			return;
		m_ScenePaths.at(index) = path;
	}

	void Game::RemoveScene(int index)
	{
		if (index < 0 || index >= m_ScenePaths.size())
			return;

		if (index != m_ScenePaths.size() - 1)
		{
			for (int i = index; i < m_ScenePaths.size() - 1; i++)
			{
				m_ScenePaths.at(i) = m_ScenePaths.at(i+1);
			}
		}
		
		m_ScenePaths.pop_back();
	}

	void Game::SaveSettings(std::filesystem::path filePath)
	{
		if (m_SettingsPath == "")
		{
			m_SettingsPath = filePath;
		}

		FileStreamWriter fileStream(filePath, false);

		fileStream.WriteString(m_Title);
		
		fileStream.WriteRaw<size_t>(m_ScenePaths.size());
		for (auto path : m_ScenePaths)
		{
			fileStream.WriteString(path.string());
		}
	}

	void Game::LoadSettings(std::filesystem::path filePath)
	{
		if (m_SettingsPath == "")
		{
			m_SettingsPath = filePath;
		}

		FileStreamReader fileStream(filePath, false);

		fileStream.ReadString(m_Title);

		if (!m_ScenePaths.empty())
			m_ScenePaths.clear();

		size_t arraySize = 0;
		fileStream.ReadRaw<size_t>(arraySize);

		m_ScenePaths.reserve(arraySize);
		for (int i = 0; i < arraySize; i++)
		{
			std::string path = "";
			fileStream.ReadString(path);
			m_ScenePaths.push_back(path);
		}
	}

	//==========================
	// Game State Machine
	//==========================

	void GameStateMachine::UpdateState(float deltaTime)
	{
		if (m_CurrentState == nullptr) return;
		m_CurrentState->UpdateState(deltaTime);
	}

	void GameStateMachine::OnImGuiRender()
	{
		if (m_CurrentState == nullptr) return;
		m_CurrentState->OnImGuiRender();
	}

	void GameStateMachine::SwitchState(const std::string_view& stateType)
	{
		if (m_States.find(stateType) != m_States.end())
		{
			EngineContext* engine = EngineContext::GetEngine();
			//found state
			std::string_view oldStateType = "";
			GameState* nextState = m_States.at(stateType);
			if (m_CurrentState != nullptr)
			{
				oldStateType = m_CurrentState->GetType();
				
				// TODO send event about state change state exit
				m_CurrentState->ExitState(nextState);
				GameStateExitedEvent exit = GameStateExitedEvent(oldStateType, stateType);
				engine->OnEvent(exit);
			}

			m_PreviousState = m_CurrentState;

			m_CurrentState = nextState;
			// this is where a state change state enter event would be sent
			m_CurrentState->EnterState(m_PreviousState);
			GameStateEnteredEvent entered = GameStateEnteredEvent(oldStateType, stateType);
			engine->OnEvent(entered);
			return;
		}

		Logger::Core(LOG_WARNING, "State ({}) does not exist in state machine.", stateType);
	}

	GameState* GameStateMachine::GetStateByType(const std::string_view& stateType)
	{
		if (m_States.find(stateType) != m_States.end())
			return m_States[stateType];
		else
			return nullptr;
	}

}