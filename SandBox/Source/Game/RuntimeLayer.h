#pragma once
#include "Layers/ImGuiLayer.h"
#include "Scene/GameObject.h"
#include "Events/KeyboardEvents.h"

namespace Pixie
{
	class EngineContext;
	class ExampleGame;
	class ConsoleWindow;

	class RuntimeLayer : public ImGuiLayer
	{
	public:
		RuntimeLayer() {}
		~RuntimeLayer() {}

		virtual std::shared_ptr<Game> GetGame() override;
		

		virtual void OnAttach() override;

		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(float deltaTime) override;

		virtual void OnImGuiRender() override;

		virtual glm::vec2 GetGameplayWindowPos() override { return m_GameplayWindowPos; }

	protected:
		virtual bool OnSceneChangedEvent(SceneChangedEvent& event) override;
	private:
		bool m_GameSelected{ false };

		glm::vec2 m_GameplayWindowPos{ 0.0f };
		std::shared_ptr<ExampleGame> m_Game;

		std::shared_ptr<Scene> m_CurrentScene{ nullptr };
		std::string m_CurrentScenePath{ "" };

		std::shared_ptr<ConsoleWindow> m_ConsoleWindow{ nullptr };
		void DrawViewport(EngineContext& engine);

	};

}