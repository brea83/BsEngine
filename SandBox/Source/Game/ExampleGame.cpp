#include "ExampleGame.h"
#include "BsPrecompileHeader.h"
#include "EngineContext.h"
#include "Scene/Components/Component.h"

#include "../Game/StateMachine/GameStates.h"
#include "CollisionBehavior/PointCollector.h"

namespace Pixie
{
	ExampleGame::~ExampleGame()
	{
		if (m_InputSystem)
		{
			delete m_InputSystem;
			m_InputSystem = nullptr;
		}
	}
	void ExampleGame::OnCreate()
	{
		std::unordered_map<std::string_view, GameState*> states;
		states.emplace(PauseState::Type(), new PauseState());
		states.emplace(PlayingState::Type(), new PlayingState());
		states.emplace(EndLevelState::Type(), new EndLevelState());
		states.emplace(TitleState::Type(), new TitleState());

		EngineContext* engine = EngineContext::GetEngine();
		bool bIsEditorEnabled = engine->IsEditorEnabled();
		if (bIsEditorEnabled)
		{
			states.emplace(EditState::Type(), new EditState());
			m_CurrentScene = engine->GetScene();
		}

		m_GameStateMachine = GameStateMachine(states);

		if (bIsEditorEnabled)
			m_GameStateMachine.SwitchState(EditState::Type());
		else
			m_GameStateMachine.SwitchState(PauseState::Type());

		m_InputSystem = new PlayerInputSystem();

		Player::RegisterToScriptManager();
		PointCollector::RegisterToScriptManager();
	}

	void ExampleGame::OnBeginPlay(std::shared_ptr<Scene> scene)
	{
		m_CurrentScene = scene;
		FindAllPlayers();
		m_CurrentScene->BeginPlayMode();
	}

	void ExampleGame::OnBeginPlay()
	{ 
		m_GameStateMachine.SwitchState(PlayingState::Type());
		//create or find players

	}
	void ExampleGame::OnUpdate(float deltaTime)
	{
		m_GameStateMachine.UpdateState(deltaTime);
	}


	bool ExampleGame::OnEvent(Event & event)
	{
		EventDispatcher dispatcher{ event };
		dispatcher.Dispatch<SceneChangedEvent>(BIND_EVENT_FUNCTION(ExampleGame::OnSceneChangedEvent));
		dispatcher.Dispatch<GameStateChangeRequestEvent>(BIND_EVENT_FUNCTION(ExampleGame::OnGameStateRequest));

		if (m_CurrentScene == nullptr)
			return false;

		m_CurrentScene->OnEvent(event);

		if (GetCurrentState() == nullptr || GetCurrentState()->GetType() != PlayingState::Type())
			return event.Handled;

		entt::registry& registry = m_CurrentScene->GetRegistry();
		auto view = registry.view<PlayerInputComponent>();
		for (auto entity : view)
		{
			PlayerInputComponent& component = view.get<PlayerInputComponent>(entity);
			//TODO will need to do something to map a control surface to a particular player this only works for one player 
			m_InputSystem->OnEvent(m_CurrentScene, component, event);
		}

		return event.Handled;
	}

	void ExampleGame::FindAllPlayers()
	{
		if (m_CurrentScene == nullptr)
			return;
		m_Players.clear();

		entt::registry& registry = m_CurrentScene->GetRegistry();

		for (auto&& [entity, player, id] : registry.view<Player, IDComponent>().each())
		{
			m_Players.push_back(id.ID);
		}

	}

	bool ExampleGame::OnSceneChangedEvent(SceneChangedEvent& event)
	{
		m_CurrentScene = event.GetScene();

		GameState* currentState = GetCurrentState();
		if (currentState == nullptr)
			return false;
		
		std::string_view stateType = currentState->GetType();

		if (stateType == PlayingState::Type() || stateType == TitleState::Type())
		{
			OnBeginPlay(m_CurrentScene);
		}
		else if (stateType == EditState::Type())
		{
			m_CurrentScene->EditMode();
		}
		else if (stateType == PauseState::Type() || stateType == EndLevelState::Type())
		{
			m_CurrentScene->Pause();
		}

		return false;
	}

	bool ExampleGame::OnGameStateRequest(GameStateChangeRequestEvent& event)
	{
		//Todo some kind of state change requestor validation
		if (event.GetState() == PlayingState::Type())
			UnPause();
		else if (event.GetState() == PauseState::Type())
			Pause();
		else if (event.GetState() == TitleState::Type())
		{
			TitleState* title = dynamic_cast<TitleState*>(m_GameStateMachine.GetStateByType(TitleState::Type()));
			if (title)
				title->InitTitleMenu();
			SetState(event.GetState());
		}
		else
			SetState(event.GetState());

		return true;
	}


	void ExampleGame::Pause()
	{
		m_GameStateMachine.SwitchState(PauseState::Type());
	}
	void ExampleGame::UnPause()
	{
		m_GameStateMachine.SwitchState(PlayingState::Type());
	}
	void ExampleGame::SetState(const std::string_view& stateType)
	{
		m_GameStateMachine.SwitchState(stateType);
	}

	void ExampleGame::OnPlayerReachedEnd(const PlayerData& data, std::filesystem::path nextLevelPath)
	{
		m_LevelData[m_CurrentLevel].Scores = data;

		if (nextLevelPath == "")
		{
			Logger::Game(LOG_DEBUG, "PLAYER DIED");
		}

		EndLevelState* endState = dynamic_cast<EndLevelState*>(m_GameStateMachine.GetStateByType(EndLevelState::Type()));
		if (endState)
			endState->InitEndMenu(nextLevelPath.string());

		GameStateChangeRequestEvent event{ Pixie::EndLevelState::Type(), "Example Game, Player reached an end" };
		EngineContext::GetEngine()->OnEvent(event);
	}

	GUID ExampleGame::GetPlayerID(int index)
	{
		if (m_Players.empty() || m_CurrentScene == nullptr || index < 0 || index >= m_Players.size())
			return 0;
		else
			return m_Players[index];
	}

	PlayerData ExampleGame::GetCurrentPlayerData()
	{
		if (m_LevelData.find(m_CurrentLevel) != m_LevelData.end() && m_LevelData[m_CurrentLevel].FilePathIndex != -1)
			return m_LevelData[m_CurrentLevel].Scores;
		else
			return PlayerData();
	}

	void ExampleGame::RequestLevelChange(int levelIndex)
	{
		EngineContext* engine = Pixie::EngineContext::GetEngine();

		int sceneIndex = m_LevelData[levelIndex].FilePathIndex;
		std::filesystem::path scenePath = m_ScenePaths[sceneIndex];

		engine->RequestSceneChange(scenePath);
	}

	void ExampleGame::RequestStateChange(std::string_view stateType)
	{
		EngineContext* engine = Pixie::EngineContext::GetEngine();
		Pixie::GameStateChangeRequestEvent event{ stateType, "ExampleGame, Request state change" };
		engine->OnEvent(event);
	}

	void ExampleGame::SaveSettings(std::filesystem::path filePath)
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

		fileStream.WriteMap<int, Level>(m_LevelData);
	}

	void ExampleGame::LoadSettings(std::filesystem::path filePath)
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

		fileStream.ReadMap <int, Level>(m_LevelData);

	}
	
}