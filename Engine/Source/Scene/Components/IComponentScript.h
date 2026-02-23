#pragma once

namespace Pixie
{
	class GameObject;
	struct CollisionEvent;
	//class IComponentScript
	//{
	//public:
	//	virtual ~IComponentScript() = 0;
	//	virtual void RegisterToScriptManager() const = 0;
	//	virtual void AddMyComponentToGameObject(GameObject& hostObject) = 0;

	//	virtual std::string_view GetName() const = 0;

	//	virtual void OnUpdate(GameObject& caller, float deltaTime) = 0;
	//	virtual void OnCollisionStart(GameObject& caller, CollisionEvent& collision) = 0;
	//	virtual void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime) = 0;
	//	virtual void DrawComponent(GameObject& selected) = 0;
	//	
	//};

	class TestComponentScript 
	{
	public:
		TestComponentScript() = default;
		TestComponentScript(const TestComponentScript&) = default;
		virtual ~TestComponentScript() { }

		// Inherited via IComponentScript
		void RegisterToScriptManager() const ;
		static void AddMyComponentToGameObject(GameObject& hostObject);
		static std::string_view GetName() { return m_Name; }

		static void OnUpdate(GameObject& caller, float deltaTime) ;
		static void OnCollisionStart(GameObject& caller, CollisionEvent& collision) ;
		static void OnCollisionOngoing(GameObject& caller, CollisionEvent& collision, float deltaTime) ;
		static void DrawComponent(GameObject& selected) ;

		
	private:
		static const std::string m_Name;

		int m_UpdateCount{ 0 };
		int m_MaxUpdates{ 10 };
		int m_CollisionCount{ 0 };
		int m_MaxCollisions{ 2 };

		float m_TimeInCollision{ 0.0f };
		
	};
}