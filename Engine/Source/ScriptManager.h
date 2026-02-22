#pragma once
#include "Scene/GameObject.h"
#include "Physics/PhysicsEngine.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
namespace Pixie
{


	class ScriptManager
	{
	public:
		static ScriptManager* GetInstance();
		~ScriptManager() { }

		std::unordered_map<std::string, std::function<void(GameObject&, float)>>& GetOnUpdateFunctions() { return m_OnUpdateLookup; }
		std::unordered_map<std::string, std::function<void(GameObject&, CollisionEvent&)>>& GetOnCollisionFunctions() { return m_OnCollisionLookup; }
		
		std::function<void(GameObject&, float)>& FindOnUpdateFunction(const std::string& name);
		std::function<void(GameObject&, CollisionEvent&)>& FindOnCollisionFunction(const std::string& name);
		std::function<void(GameObject&)>& FindDrawComponentFunction(const std::string& name);

		std::vector< std::string> GetOnUpdateFuncNames();
		std::vector< std::string> GetOnCollisionFuncNames();

		bool TryAddOnUpdateFunction(const std::string& name, std::function<void(GameObject&, float)> function);
		bool TryAddOnCollisionFunction(const std::string& name, std::function<void(GameObject&, CollisionEvent&)> function);
		bool TryAddDrawComponentFunction(const std::string& name, std::function<void(GameObject&)> drawFunction);

		void DrawComponentTest(GameObject& selected);

	protected:
		ScriptManager();

	private:
		static ScriptManager* m_Instance;

		int m_TestInt{ 0 };

		//on update functions need own gameObject, and delta time
		std::unordered_map<std::string, std::function<void(GameObject&, float)>> m_OnUpdateLookup;

		// on collision functions need the gameobject for self, and the collision event
		std::unordered_map<std::string, std::function<void(GameObject&, CollisionEvent&)>> m_OnCollisionLookup;

		std::unordered_map<std::string, std::function<void(GameObject&)>> m_DrawComponentLookup;


		std::function<void(GameObject&, float)> m_OnUpdateFuncNotFound;
		std::function<void(GameObject&, CollisionEvent&)> m_OnCollisionFuncNotFound;
		std::function<void(GameObject&)> m_DrawComponentFuncNotFound;
		//friend class DetailsViewPanel;
	};
}