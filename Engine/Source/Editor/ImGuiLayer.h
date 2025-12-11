#pragma once

#include "Layer.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/ImGuiPanel.h"
#include <glm/glm.hpp>
#include "Scene/GameObject.h"
#include "Events/KeyboardEvents.h"

namespace Pixie
{
	class EngineContext;
	class Camera;
	class Scene;

	//may need key, mouse, and application events
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnSceneChange(Scene* newScene) { m_CurrentScene = newScene; }
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void DrawViewport(EngineContext& engine, GameObject& selected);

		void DrawEditorMenu(EngineContext* engine);

		glm::vec2 GetViewportSize() const { return m_ViewportPanelSize; }

		void Begin();
		void End();

	private:
		float m_Time{ 0.0f };
		Scene* m_CurrentScene{ nullptr };
		glm::vec2 m_ViewportPanelSize{ 0.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_bViewportResized{ true };

		//SceneHierarchyPanel m_Hierarchy;
		GameObject m_Selected{ entt::null, nullptr };
		int m_GizmoType{ -1 };

		ImGuiPanel* m_AssetViewer{ nullptr };

		void DrawSceneTools();

		void DrawGridLines(Camera* camera);
		void DrawGizmos(Camera* camera, glm::mat4 viewMatrix/*Transform& camTransform*/, GameObject& selected);

		bool OnKeyPressed(KeyPressedEvent& event);
	};
}