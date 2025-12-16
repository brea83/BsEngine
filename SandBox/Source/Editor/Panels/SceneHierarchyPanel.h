#pragma once
//#include "Scene/GameObject.h"

namespace Pixie
{
	class Scene;
	class GameObject;

	class SceneHierarchyPanel
	{
	public:
		static void Draw(Scene* currentScene, GameObject& selected);
		static void DrawNode(GameObject& selected, GameObject& entity);
	private:
	};
}
