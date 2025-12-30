#pragma once
#include "Layers/ImGuiLayer.h"
#include "Scene/GameObject.h"
#include "Events/KeyboardEvents.h"


namespace Pixie
{
	class EngineContext;
	class Camera;
	class Scene;
	class ImGuiPanel;

	//may need key, mouse, and application events
	class EditorLayer : public ImGuiLayer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnSceneChange(Scene* newScene, const std::string& filepath = "")
		{ 
			m_CurrentScene = newScene; 
			m_Selected = GameObject(entt::null, m_CurrentScene); 
			m_CurrentScenePath = filepath;
		}
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void DrawViewport(EngineContext& engine, GameObject& selected);

		void DrawEditorMenu(EngineContext* engine);

		void NewScene();
		void SaveScene();
		void SaveSceneAs();
		void OpenScene();


	private:
		Scene* m_CurrentScene{ nullptr };
		std::string m_CurrentScenePath{ "" };
		

		//SceneHierarchyPanel m_Hierarchy;
		GameObject m_Selected{ entt::null, nullptr };
		int m_GizmoType{ -1 };

		//ImGuiPanel* m_AssetViewer{ nullptr };

		void DrawSceneTools();
		void DrawEditorCamTools(GameObject& activeCam);

		void DrawGridLines(Camera* camera);
		void DrawGizmos(Camera* camera, glm::mat4 viewMatrix/*TransformComponent& camTransform*/, GameObject& selected);

		bool OnKeyPressed(KeyPressedEvent& event);
	};
}