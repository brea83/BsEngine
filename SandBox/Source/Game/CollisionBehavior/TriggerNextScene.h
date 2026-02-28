#pragma once
#include <string>
#include "Resources/FileStream.h"

namespace Pixie
{
	class ExampleGame;
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
		static void CopyComponent(GameObject& sourceObject, GameObject& destinationObject);
		static std::string_view GetName() { return m_Name; }

		static void OnUpdate(GameObject& caller, float deltaTime);
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision);
		static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime);
		static void Draw(GameObject& selected);

		static void Serialize(StreamWriter* stream, const GameObject& sourceObject);
		static bool Deserialize(StreamReader* stream, GameObject& destinationObject);

	private:
		static const std::string m_Name;
		std::shared_ptr<ExampleGame> m_Game;

		bool triggered{ false };
		std::string m_NextSceneName{ "MovementTesting" };
		std::string m_TagThatActivatesTrigger{ "Player" };

		int m_NextLevelIndex{ 0 };


		void DrawLevelSelect(GameObject& selected);
	};

}