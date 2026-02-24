#pragma once
#include "Scene/GameObject.h"
#include "Physics/PhysicsEngine.h"
#include "Scene/Components/IComponentScript.h"
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
		
		

		//bool TryAttachScriptComponent(GameObject& caller, const std::string& name);
		bool FindStoredScript(const std::string& name, StoredScript& outScriptFunctions);

		bool FindOnUpdateFunction(const std::string& name, std::function<void(GameObject&, float)>& function);
		bool FindOnCollisionFunction(const std::string& name, std::function<void(GameObject&, CollisionEvent&)>& function);
		bool FindDrawComponentFunction(const std::string& name, std::function<void(GameObject&)>& function);
		bool FindAttachComponentFunction(const std::string& name, std::function<void(GameObject&)>& function);
		bool FindRemoveComponentFunction(const std::string& name, std::function<void(GameObject&)>& function);
		bool FindCopyComponentFunction(const std::string& name, std::function<void(GameObject&, GameObject&)>& function);
		/*bool FindSerializeComponentFunction(const std::string& name, std::function<void(StreamWriter*, const GameObject&)> function);
		bool FindDeserializeComponentFunction(const std::string& name, std::function<void(StreamReader*, const GameObject&)> function);*/

		std::vector< std::string> GetScriptNames();
		std::vector< std::string> GetOnUpdateFuncNames();
		std::vector< std::string> GetOnCollisionFuncNames();

		bool TryAddAttatchComponentFunction(const std::string& name, std::function<void(GameObject&)> attachFunction);
		bool TryAddRemoveComponentFunction(const std::string& name, std::function<void(GameObject&)> removeFunction);
		bool TryAddOnUpdateFunction(const std::string& name, std::function<void(GameObject&, float)> function);
		bool TryAddOnCollisionFunction(const std::string& name, std::function<void(GameObject&, CollisionEvent&)> function);
		bool TryAddDrawComponentFunction(const std::string& name, std::function<void(GameObject&)> drawFunction);
		bool TryAddCopyComponentFunction(const std::string& name, std::function<void(GameObject&, GameObject&)> function);
		
		bool TryStoreScript(const std::string& name, StoredScript scriptFunctions);
		
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

		std::unordered_map<std::string, std::function<void(GameObject&)>> m_AttachComponentLookup;
		std::unordered_map<std::string, std::function<void(GameObject&)>> m_RemoveComponentLookup;
		std::unordered_map<std::string, std::function<void(GameObject&, GameObject&)>> m_CopyComponentLookup;

		//std::unordered_map<std::string, std::function<void(StreamWriter*, const GameObject&)>> m_SerializeComponentLookup;
		std::unordered_map<std::string, StoredScript> m_StoredScripts;

		std::function<void(GameObject&, float)> m_OnUpdateFuncNotFound;
		std::function<void(GameObject&, CollisionEvent&)> m_OnCollisionFuncNotFound;
		std::function<void(GameObject&)> m_DrawComponentFuncNotFound;
		//friend class DetailsViewPanel;
	};
}