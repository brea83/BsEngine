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

		if (currentState->GetType() == PlayingState::Type())
		{
			OnBeginPlay(m_CurrentScene);
		}

		if (currentState->GetType() == EditState::Type())
			m_CurrentScene->EditMode();

		if (currentState->GetType() == PauseState::Type())
			m_CurrentScene->Pause();

		return false;
	}

	bool ExampleGame::OnGameStateRequest(GameStateChangeRequestEvent& event)
	{
		//Todo some kind of state change requestor validation
		if (event.GetState() == PlayingState::Type())
			UnPause();
		else if (event.GetState() == PauseState::Type())
			Pause();
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

	void ExampleGame::OnLevelEnd(const PlayerData& data, std::filesystem::path nextLevelPath)
	{
		m_LevelData[m_CurrentLevel].Scores = data;

		if (nextLevelPath == "")
		{
			Logger::Game(LOG_DEBUG, "PLAYER DIED");
			GameStateChangeRequestEvent event{ Pixie::PauseState::Type(), "Game, Player Death" };
			EngineContext::GetEngine()->OnEvent(event);
		}
		else
		{
			EngineContext::GetEngine()->RequestSceneChange(nextLevelPath);
		}
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
	
}