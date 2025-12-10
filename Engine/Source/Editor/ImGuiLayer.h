#pragma once

#include "Layer.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/ImGuiPanel.h"
#include <glm/glm.hpp>

namespace Pixie
{
	class EngineContext;
	class Camera;
	class Scene;
}
	//may need key, mouse, and application events
	class ImGuiLayer : public Pixie::Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnSceneChange(Pixie::Scene* newScene) { m_CurrentScene = newScene; }
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void DrawViewport(Pixie::EngineContext& engine, int selected);

		void DrawEditorMenu(Pixie::EngineContext* engine);

		void Begin();
		void End();

	private:
		float m_Time{ 0.0f };
		Pixie::Scene* m_CurrentScene;
		glm::vec2 m_ViewportPanelSize{ 0.0f };

		SceneHierarchyPanel m_Hierarchy;
		ImGuiPanel* m_AssetViewer;

		void DrawSceneTools();

		void DrawGridLines(std::shared_ptr<Pixie::Camera> camera);
		void DrawGizmos(std::shared_ptr<Pixie::Camera> camera, int selectedObjectIndex);

	};
