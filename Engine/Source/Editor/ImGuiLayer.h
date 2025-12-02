#pragma once

#include "Layer.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/ImGuiPanel.h"
#include <glm/glm.hpp>

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

		void DrawViewport(EngineContext& engine, int selected);

		void DrawEditorMenu(EngineContext* engine);

		void Begin();
		void End();

	private:
		float m_Time{ 0.0f };
		Scene* m_CurrentScene;
		glm::vec2 m_ViewportPanelSize{ 0.0f };

		SceneHierarchyPanel m_Hierarchy;
		ImGuiPanel* m_AssetViewer;

		void DrawSceneTools();

		void DrawGridLines(std::shared_ptr<Camera> camera);
		void DrawGizmos(std::shared_ptr<Camera> camera, int selectedObjectIndex);

	};
