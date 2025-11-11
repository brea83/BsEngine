#pragma once

#include "../Layer.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/ImGuiPanel.h"

class Scene;

	//may need key, mouse, and application events
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnSceneChange(Scene* newScene) { _hierarchy.SetContext(newScene); }
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float _time{ 0.0f };
		SceneHierarchyPanel _hierarchy;
		ImGuiPanel* _assetViewer;
	};
