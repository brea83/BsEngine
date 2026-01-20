#pragma once
//#include "Scene/GameObject.h"

namespace Pixie
{
	class Scene;
	class GameObject;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		void Draw();
		void DrawNode(GameObject& entity);
		std::shared_ptr<GameObject> GetSelected() { return m_Selected; }
		void OnSceneChange(Scene* newScene);
	private:
		Scene* m_CurrentScene{ nullptr };
		std::shared_ptr<GameObject> m_Selected{ nullptr };

		std::string m_HeirarchyDragType{ "HEIRARCHY_ITEM" };
	};
}
