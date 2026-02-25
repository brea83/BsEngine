#pragma once
#include "Scene/Components/IComponentScript.h"
#include "Scene/Components/Component.h"
#include <functional>

namespace Pixie
{
	class PointCollector
	{
	public:
		PointCollector() = default;
		PointCollector(const PointCollector&) = default;
		
		GUID ID{ 0 };

		static void RegisterToScriptManager();
		static void Attach(GameObject& hostObject);
		static void Remove(GameObject& hostObject);
		static void Copy(GameObject& sourceObject, GameObject& destinationObject);
		static std::string_view GetName() { return m_Name; }

		static void OnUpdate(GameObject& caller, float deltaTime);
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
		static void Draw(GameObject& selected);

		static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
		static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

		void SetOnPointsCollectedCallback(std::function<void(int)> callback) { m_OnPointsCollectedCallback = callback; }
	private:
		static const std::string m_Name;

		std::function<void(int)> m_OnPointsCollectedCallback{};
		// todo add a mesh component pointer for visualization of range durring play
	};
}

