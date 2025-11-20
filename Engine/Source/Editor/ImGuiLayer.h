#pragma once

#include "../Layer.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/ImGuiPanel.h"

class EngineContext;

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
		SceneHierarchyPanel _hierarchy;
		ImGuiPanel* _assetViewer;
	};
