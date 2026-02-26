#include "GameStates.h"
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "../ExampleGame.h"

namespace Pixie
{
	//==========================
	// Pause state
	//==========================
	const std::string PauseState::m_Type = "Pause";

	void PauseState::EnterState(GameState* previousState)
	{
		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr) 
			return;
		
		scene->Pause();
		m_PauseMenu.SetCurrentScene(scene);
		
	}

	void PauseState::ExitState(GameState* nextState)
	{}

	void PauseState::UpdateState(float deltaTime)
	{
		//ToDo might want to add an updates on pause component
	}

	void PauseState::OnImGuiRender()
	{
		m_PauseMenu.Draw();
	}

	//==========================
	// Play state
	//==========================
	const std::string PlayingState::m_Type = "Playing";

	void PlayingState::EnterState(GameState* previousState)
	{
		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr) return;

		if (previousState == nullptr || previousState->GetType() != PauseState::Type())
			scene->BeginPlayMode();
		else
			scene->UnPause();
	}

	void PlayingState::ExitState(GameState* nextState)
	{
		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr) return;

		if (nextState == nullptr || nextState->GetType() != PauseState::Type())
			scene->EndPlayMode();

	}

	void PlayingState::UpdateState(float deltaTime)
	{

		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr) return;
		scene->OnUpdate(deltaTime);
	}

	//==========================
	// EDIT state
	//==========================
	const std::string EditState::m_Type = "Edit";

	void EditState::EnterState(GameState* previousState)
	{

		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr)
			return;

		scene->EditMode();
	}

	void EditState::ExitState(GameState * nextState)
	{}

	void EditState::UpdateState(float deltaTime)
	{

		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr) 
			return;

		scene->OnEditorUpdate(deltaTime);
	}

	//=========================================
	// END LEVEL STATE
	//=========================================
	const std::string EndLevelState::m_Type = "EndLevel";

	void EndLevelState::EnterState(GameState* previousState)
	{
		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr)
			return;

		scene->Pause();
	}
	void EndLevelState::ExitState(GameState * nextState)
	{}
	void EndLevelState::UpdateState(float deltaTime)
	{}
	void EndLevelState::OnImGuiRender()
	{
		m_EndLevelMenu.Draw();
	}
	void EndLevelState::InitEndMenu(const std::string & nextLevelPath)
	{
		m_EndLevelMenu.Init(nextLevelPath);
	}

	//=========================================
	// TITLE MENU STATE
	//=========================================
	const std::string TitleState::m_Type = "TitleState";

	void TitleState::EnterState(GameState* previousState)
	{
		if (m_Game)
			m_Game->RequestLevelChange(0);
	}

	void TitleState::ExitState(GameState * nextState)
	{ }

	void TitleState::UpdateState(float deltaTime)
	{
		std::shared_ptr<Scene> scene = EngineContext::GetEngine()->GetScene();

		if (scene == nullptr) return;
		scene->OnUpdate(deltaTime);
	}

	void TitleState::OnImGuiRender()
	{
		m_TitleMenu.Draw();
	}

	void TitleState::InitTitleMenu()
	{
		m_TitleMenu.Init();
	}
}