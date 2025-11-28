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

		void OnSceneChange(Scene* newScene) { _currentScene = newScene; }
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void DrawEditorMenu(EngineContext* engine);

		void Begin();
		void End();

	private:
		float _time{ 0.0f };
		Scene* _currentScene;
		glm::vec2 _viewportPanelSize{ 0.0f };

		bool bGridVisibleX{ true };
		bool bGridVisibleY{ false };
		bool bGridVisibleZ{ true };
		SceneHierarchyPanel _hierarchy;
		ImGuiPanel* _assetViewer;

		void DrawSceneTools();

		void DrawGridLines(Camera* camera);
		void DrawGizmos(Camera* camera, int selectedObjectIndex);

	};
