#pragma once
#include <string>
namespace Pixie
{
	class GameObject;
	struct CollisionEvent;

	class TriggerNextScene
	{
	public:
		TriggerNextScene() = default;
		TriggerNextScene(const TriggerNextScene&) = default;
		~TriggerNextScene() { }

		static void RegisterToScriptManager();
		static void AddMyComponentToGameObject(GameObject& hostObject);
		static void RemoveMyComponent(GameObject& hostObject);
		static std::string_view GetName() { return m_Name; }

		static void OnUpdate(GameObject& caller, float deltaTime);
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
		static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime);
		static void Draw(GameObject& selected);

	private:
		static const std::string m_Name;

		bool triggered{ false };
		std::string m_NextSceneName{ "MovementTesting" };
		std::string m_TagThatActivatesTrigger{ "Player" };
	};

}