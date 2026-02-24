#include "Damageable.h"
#include "ScriptManager.h"

namespace Pixie
{
	const std::string Damageable::m_Name = "Damageable";
	void Damageable::RegisterToScriptManager()
	{
		ScriptManager* scripts = ScriptManager::GetInstance();

		using std::placeholders::_1;
		using std::placeholders::_2;
		using std::placeholders::_3;
		StoredScript newScript;
		newScript.AttachComponent = std::bind(&Damageable::AddMyComponentToGameObject, _1);
		newScript.CopyComponent = std::bind(&Damageable::CopyComponent, _1, _2);
		newScript.RemoveComponent = std::bind(&Damageable::RemoveMyComponent, _1);
		newScript.Serialize = std::bind(&Damageable::Serialize, _1, _2);
		newScript.Deserialize = std::bind(&Damageable::Deserialize, _1, _2);
		newScript.Draw = std::bind(&Damageable::Draw, _1);
		newScript.OnUpdate = std::bind(&Damageable::OnUpdate, _1, _2);
		newScript.OnCollision = std::bind(&Damageable::OnCollisionStart, _1, _2);
		newScript.OnCollisionOngoing = std::bind(&Damageable::OnCollisionOngoing, _1, _2, _3);

		scripts->TryStoreScript(m_Name, newScript);
	}

	void Damageable::AddMyComponentToGameObject(GameObject & hostObject)
	{
		hostObject.AddComponent < Damageable>();
	}

	void Damageable::RemoveMyComponent(GameObject & hostObject)
	{
		hostObject.RemoveComponent<Damageable>();
	}

	void Damageable::CopyComponent(GameObject & sourceObject, GameObject & destinationObject)
	{
		if (!sourceObject.HasCompoenent<Damageable>())
			return;
		Damageable& source = sourceObject.GetComponent<Damageable>();

		destinationObject.AddOrReplaceComponent<Damageable>(source);
	}

	void Damageable::OnUpdate(GameObject & caller, float deltaTime)
	{
		Damageable& component = caller.GetComponent<Damageable>();
		component.Update(caller, deltaTime);
	}

	void Damageable::Update(GameObject& caller, float deltaTime)
	{
		if (m_CurrentHealth <= 0)
		{
			caller.OnDestroy();
			m_DamageSourcesThisFrame.clear();
			return;
		}

		if (m_IsInvulnerable == true)
		{
			if (m_AccumulatedIFrames <= m_IFrames)
				m_AccumulatedIFrames++;
			else
			{
				m_AccumulatedIFrames = 0;
				m_IsInvulnerable = false;
			}
		}
		else
		{
			TakeDamage();
			if (m_IFrames > 0 && !m_DamageSourcesThisFrame.empty())
				m_IsInvulnerable = true;
		}

		m_DamageSourcesThisFrame.clear();
	}

	void Damageable::OnCollisionStart(GameObject & caller, CollisionEvent & collision)
	{
		Damageable& component = caller.GetComponent<Damageable>();
		if (component.m_IsInvulnerable)
			return;

		GameObject other = ExtractOtherObject(caller, collision);
		if (component.TestCollisionValid(caller, other))
		{
			component.CollectDamage(caller, other);
		}
	}

	void Damageable::OnCollisionOngoing(GameObject & caller, CollisionEvent & collision, float deltaTime)
	{
		Damageable& component = caller.GetComponent<Damageable>();
	}

	void Damageable::Draw(GameObject & selected)
	{
		Damageable& component = selected.GetComponent<Damageable>();
	}

	void Damageable::Serialize(StreamWriter * stream, const GameObject & sourceObject)
	{
		Damageable& component = sourceObject.GetComponent<Damageable>();
	}

	bool Damageable::Deserialize(StreamReader * stream, GameObject & destinationObject)
	{
		Damageable& component = destinationObject.GetComponent<Damageable>();
		return false;
	}
	GameObject Damageable::ExtractOtherObject(GameObject thisObject, CollisionEvent& collision)
	{
		GUID myId = thisObject.GetGUID();
		Entity other = Entity();
		if (collision.A.GetComponent<IDComponent>().ID == myId)
			other = collision.B;
		else
			other = collision.A;

		return GameObject(other.GetEnttHandle(), other.GetScene());
	}
	bool Damageable::TestCollisionValid(GameObject& thisObject, GameObject& other)
	{
		

		if (other.TryGetComponent<TagComponent>() == nullptr)
			return false;

		std::string othersTag = other.GetComponent<TagComponent>().Tag;

		if (std::find(m_TagsThatDamageThis.begin(), m_TagsThatDamageThis.end(), othersTag) != m_TagsThatDamageThis.end())
		{
			return true;
		}

		return false;
	}
	void Damageable::CollectDamage(GameObject& thisObject, GameObject& other)
	{
		GUID id = other.GetGUID();
		if (m_DamageSourcesThisFrame.find(id) != m_DamageSourcesThisFrame.end())
		{
			return; // allready took damage from this source this frame
		}

		int incomingDamage = 0;
		if (other.HasCompoenent<Attack>())
		{
			incomingDamage = other.GetComponent<Attack>().Damage;
		}
		else 
		{
			incomingDamage = 1;
		}

		m_DamageSourcesThisFrame[id] = incomingDamage;
	}
	void Damageable::TakeDamage()
	{
		for (auto pair : m_DamageSourcesThisFrame)
		{
			m_CurrentHealth -= pair.second;
		}

	}
}