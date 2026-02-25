#pragma once
#include "GUID.h"
#include <ScriptManager.h>

namespace Pixie
{
	class GameObject;
};
// these are Used in script component to access functions, but also allow serialization

struct Attack 
{
	int Damage{ 1 };
	GUID Owner{ 0 };

		static void RegisterToScriptManager();
		static void Attach(Pixie::GameObject& hostObject);
		static void Remove(Pixie::GameObject& hostObject);
		static void Copy(Pixie::GameObject& sourceObject, Pixie::GameObject& destinationObject);
		static std::string_view GetName() { return m_Name; }
	
		static void Draw(Pixie::GameObject& selected);
	
		static void Serialize(Pixie::StreamWriter* stream, const Pixie::GameObject& sourceObject);
		static bool Deserialize(Pixie::StreamReader* stream, Pixie::GameObject& destinationObject);

	private:
		static const std::string m_Name;
};

struct ScoresPoints 
{
	int Value{ 1 };
	bool BPointsDeliveredOnDeath{ false };
	bool BPointsDeliveredOnCollision{ true };

	static void RegisterToScriptManager();
	static void Attach(Pixie::GameObject& hostObject);
	static void Remove(Pixie::GameObject& hostObject);
	static void Copy(Pixie::GameObject& sourceObject, Pixie::GameObject& destinationObject);
	static std::string_view GetName() { return m_Name; }

	static void OnBeginPlay(Pixie::GameObject& hostObject);

	static void Draw(Pixie::GameObject& selected);

	static void Serialize(Pixie::StreamWriter* stream, const Pixie::GameObject& sourceObject);
	static bool Deserialize(Pixie::StreamReader* stream, Pixie::GameObject& destinationObject);

	void OnDeath(Pixie::GUID& killerGuid);
private:
	static const std::string m_Name;
	Pixie::GUID m_OwnId{ 0 };
	std::shared_ptr<Pixie::Scene> m_CurrentScene{ nullptr };
};
