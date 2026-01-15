#pragma once
#include "Layers/ImGuiLayer.h"
#include "Scene/GameObject.h"
#include "Events/KeyboardEvents.h"


namespace Pixie
{
	class EngineContext;
	class Camera;
	class Scene;
	class ConsoleWindow;
	class RenderInspectorPanel;

	//may need key, mouse, and application events
	class EditorLayer : public ImGuiLayer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnSceneChange(Scene* newScene, const std::string& filepath = "");
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void OnScenePlay();
		void OnScenePause();
		void OnSceneStop();
		virtual void OnUpdate(float deltaTime) override;

		void NewScene();
		void SaveScene();
		void SaveSceneAs();
		void OpenScene();

		virtual void OnImGuiRender() override;

	private:
		Scene* m_CurrentScene{ nullptr };
		std::string m_CurrentScenePath{ "" };
		SceneState m_SceneState{SceneState::Edit};
		std::string m_PlayPauseText{ "Play" };


		//SceneHierarchyPanel m_Hierarchy;
		GameObject m_Selected{ entt::null, nullptr };
		int m_GizmoType{ -1 };

		std::shared_ptr<ConsoleWindow> m_ConsoleWindow{ nullptr };
		
		bool m_ShowRenderInspector{ false };
		std::shared_ptr<RenderInspectorPanel> m_RenderInspecorPanel{ nullptr };


		void DrawMainMenu(EngineContext* engine);
		void DrawMainMenuBar2();

		void DrawEditorCamTools(GameObject& activeCam);
		
		void DrawViewport(EngineContext& engine, GameObject& selected);

		void DrawGridLines(Camera* camera);
		void DrawGizmos(Camera* camera, glm::mat4 viewMatrix/*TransformComponent& camTransform*/, GameObject& selected);

		bool OnKeyPressed(KeyPressedEvent& event);
	};
}