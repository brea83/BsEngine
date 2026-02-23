#include "BsPrecompileHeader.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"


namespace Pixie
{
	ScriptManager* ScriptManager::m_Instance = nullptr;

	ScriptManager::ScriptManager()
	{
		m_OnUpdateFuncNotFound = [](GameObject& caller, float deltaTime)
		{
			Logger::Core(LOG_WARNING, "No OnUpdate found for { }", caller.GetName());
		};
		m_OnCollisionFuncNotFound = [](GameObject& caller, CollisionEvent& collision)
		{
			Logger::Core(LOG_WARNING, "No OnCollision found for { }", caller.GetName());
		};
		m_DrawComponentFuncNotFound = [](GameObject& caller)
		{
			Logger::Core(LOG_WARNING, "No Draw Component found for one of selected, {}'s, native script component functions", caller.GetName());
		};
		/*
		TryAddOnUpdateFunction("Test OnUpdate Lambda", [](GameObject& caller, float deltatime)
		{
			Logger::Core(LOG_DEBUG, "This is a test of storing a lambda in the function look up.");
			Logger::Core(LOG_DEBUG, "Calling object is: {}, and the deltaTime is {}", caller.GetName(), deltatime);
		});

		using std::placeholders::_1;
		TryAddDrawComponentFunction("Test OnUpdate Lambda", std::bind(&ScriptManager::DrawComponentTest, this, _1));*/

	}

	ScriptManager* ScriptManager::GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new ScriptManager();
			return m_Instance;
		}
		return m_Instance;
	}

	bool ScriptManager::TryAttachScriptComponent(GameObject& caller, const std::string& name)
	{
		if (m_AttachComponentLookup.find(name) != m_AttachComponentLookup.end())
		{
			m_AttachComponentLookup[name](caller);
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find OnUpdate function named {}", name);

		return false;
	}

	 bool ScriptManager::FindOnUpdateFunction(const std::string& name, std::function<void(GameObject&, float)>& function)
	{
		if (m_OnUpdateLookup.find(name) != m_OnUpdateLookup.end())
		{
			function = m_OnUpdateLookup[name];
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find OnUpdate function named {}", name);

		return false;
	}

	bool ScriptManager::FindOnCollisionFunction(const std::string& name, std::function<void(GameObject&, CollisionEvent&)>& function)
	{
		if (m_OnCollisionLookup.find(name) != m_OnCollisionLookup.end())
		{
			function = m_OnCollisionLookup[name];
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find OnCollision function named {}", name);

		return false;
	}

	bool ScriptManager::FindDrawComponentFunction(const std::string& name, std::function<void(GameObject&)>& function)
	{
		if (m_DrawComponentLookup.find(name) != m_DrawComponentLookup.end())
		{
			function = m_DrawComponentLookup[name];
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find Draw Component function named {}", name);

		return false;
	}

	bool ScriptManager::FindRemoveComponentFunction(const std::string& name, std::function<void(GameObject&)>& function)
	{
		if (m_RemoveComponentLookup.find(name) != m_RemoveComponentLookup.end())
		{
			function = m_RemoveComponentLookup[name];
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find Remove Component function named {}", name);

		return false;
	}

	bool ScriptManager::FindCopyComponentFunction(const std::string& name, std::function<void(GameObject&, GameObject&)>& function)
	{
		if (m_CopyComponentLookup.find(name) != m_CopyComponentLookup.end())
		{
			function = m_CopyComponentLookup[name];
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find Remove Component function named {}", name);

		return false;
	}

	bool ScriptManager::FindAttachComponentFunction(const std::string& name, std::function<void(GameObject&)>& function)
	{
		if (m_AttachComponentLookup.find(name) != m_AttachComponentLookup.end())
		{
			function = m_AttachComponentLookup[name];
			return true;
		}
		Logger::Core(LOG_WARNING, "Could not find Remove Component function named {}", name);

		return false;
	}

	std::vector< std::string> ScriptManager::GetScriptNames()
	{
		std::vector< std::string> names;
		names.push_back("None");

		for (auto pair : m_AttachComponentLookup)
		{
			names.push_back(pair.first);
		}

		return names;
	}

	std::vector< std::string> ScriptManager::GetOnUpdateFuncNames()
	{
		std::vector< std::string> names;
		names.push_back("None");

		for (auto pair : m_OnUpdateLookup)
		{
			names.push_back(pair.first);
		}

		return names;
	}

	std::vector<std::string> ScriptManager::GetOnCollisionFuncNames()
	{
		std::vector< std::string> names;
		names.push_back("None");

		for (auto pair : m_OnCollisionLookup)
		{
			names.push_back(pair.first);
		}

		return names;
	}

	bool ScriptManager::TryAddAttatchComponentFunction(const std::string& name, std::function<void(GameObject&)> attachFunction)
	{
		if (m_AttachComponentLookup.find(name) != m_AttachComponentLookup.end())
		{
			Logger::Core(LOG_WARNING, "ScriptManager is already storing an attach script component function named {}", name);
			return false;
		}
		m_AttachComponentLookup[name] = attachFunction;
		return true;
	}

	bool ScriptManager::TryAddRemoveComponentFunction(const std::string& name, std::function<void(GameObject&)> removeFunction)
	{
		if (m_RemoveComponentLookup.find(name) != m_RemoveComponentLookup.end())
		{
			Logger::Core(LOG_WARNING, "ScriptManager is already storing an remove script component function named {}", name);
			return false;
		}
		m_RemoveComponentLookup[name] = removeFunction;
		return true;
	}

	bool ScriptManager::TryAddOnUpdateFunction(const std::string& name, std::function<void(GameObject&, float)> function)
	{
		if (m_OnUpdateLookup.find(name) != m_OnUpdateLookup.end())
		{
			Logger::Core(LOG_WARNING, "ScriptManager is already storing an OnUpdated function named {}", name);
			return false;
		}
		m_OnUpdateLookup[name] = function;
		return true;
	}

	bool ScriptManager::TryAddOnCollisionFunction(const std::string& name, std::function<void(GameObject&, CollisionEvent&)> function)
	{
		if (m_OnCollisionLookup.find(name) != m_OnCollisionLookup.end())
		{
			Logger::Core(LOG_WARNING, "ScriptManager is already storing an OnCollision function named {}", name);
			return false;
		}
		m_OnCollisionLookup[name] = function;
		return true;
	}

	bool ScriptManager::TryAddDrawComponentFunction(const std::string& name, std::function<void(GameObject&)> drawFunction)
	{
		if (m_DrawComponentLookup.find(name) != m_DrawComponentLookup.end())
		{
			Logger::Core(LOG_WARNING, "ScriptManager is already storing a DrawComponent function named {}", name);
			return false;
		}
		m_DrawComponentLookup[name] = drawFunction;
		return true;
	}

	bool ScriptManager::TryAddCopyComponentFunction(const std::string& name, std::function<void(GameObject&, GameObject&)> function)
	{
		if (m_CopyComponentLookup.find(name) != m_CopyComponentLookup.end())
		{
			Logger::Core(LOG_WARNING, "ScriptManager is already storing a DrawComponent function named {}", name);
			return false;
		}
		m_CopyComponentLookup[name] = function;
		return true;
	}

	void ScriptManager::DrawComponentTest(GameObject& selected)
	{
		ImGui::TextWrapped("Test int on ScriptManager, this is where you'd describe what the func or component does");
		
		ImGui::Text("this int doesn't do anything");
		ImGui::SameLine();
		ImGui::InputInt("##dontshowlabel", &m_TestInt);

		if (selected.HasCompoenent<NameComponent>())
		{
			ImGui::Text("Selected is named: ");
			ImGui::SameLine();
			ImGui::Text(selected.GetName().c_str());
		}
	}



}