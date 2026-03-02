#pragma once
#include "Source/GameState.h"
#include "../UI/PauseMenu.h"
#include "../UI/EndMenu.h"
#include "../UI/StartMenu.h"
#include "../UI/HUD.h"

namespace Pixie
{
	class ExampleGame;

	class EndLevelState : public GameState
	{
	public:
		EndLevelState() = default;
		
		static const std::string_view Type() { return m_Type; }
		// Inherited via GameState
		const std::string_view GetType() const override { return m_Type; }
		void EnterState(GameState* previousState) override;
		void ExitState(GameState* nextState) override;
		void UpdateState(float deltaTime) override;
		void OnImGuiRender() override;

		void InitEndMenu(int levelIndex);//const std::string& nextLevelPath);
	private:
		static const std::string m_Type;
		EndMenu m_EndLevelMenu{};
	};

	// title state is a special playing state on title menu level
	class TitleState : public GameState
	{
	public:
		TitleState() = default;

		static const std::string_view Type() { return m_Type; }
		// Inherited via GameState
		const std::string_view GetType() const override { return m_Type; }
		void EnterState(GameState* previousState) override;
		void ExitState(GameState* nextState) override;
		void UpdateState(float deltaTime) override;
		void OnImGuiRender() override;

		void InitTitleMenu();
	private:
		static const std::string m_Type;
		StartMenu m_TitleMenu;
		std::shared_ptr<ExampleGame> m_Game;
	};

	class PauseState : public GameState
	{
	public:
		PauseState() = default;

		static const std::string_view Type() { return m_Type; }
		// Inherited via GameState
		const std::string_view GetType() const override { return m_Type; }
		void EnterState(GameState* previousState) override;
		void ExitState(GameState* nextState) override;
		void UpdateState(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		static const std::string m_Type;
		PauseMenu m_PauseMenu;
	};
	
	class PlayingState : public GameState
	{
	public:
		PlayingState() = default;

		static const std::string_view Type() { return m_Type; }
		// Inherited via GameState
		const std::string_view GetType() const override { return m_Type; }
		void EnterState(GameState* previousState) override;
		void ExitState(GameState* nextState) override;
		void UpdateState(float deltaTime) override;

		void OnImGuiRender() override;

		void InitHUD(glm::vec2 windowPosition, glm::vec2 windowSize);

	private:
		static const std::string m_Type;
		HUD m_HUD;
	};

	class EditState : public GameState
	{
	public:
		EditState() = default;

		static const std::string_view Type() { return m_Type; }
		// Inherited via GameState
		const std::string_view GetType() const override { return m_Type; }
		void EnterState(GameState* previousState) override;
		void ExitState(GameState* nextState) override;
		void UpdateState(float deltaTime) override;

		void OnImGuiRender() override { }

	private:
		static const std::string m_Type;
	};
}
