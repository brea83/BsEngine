#include "Damageable.h"
#include "ScriptManager.h"
#include "ImGui/ImGuiPanel.h"

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

	void Damageable::OnBeginPlay(GameObject& caller)
	{
		if (caller.HasCompoenent<Damageable>())
			caller.GetComponent<Damageable>().Reset();
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
			//TODO: this is where I would check for object pool and set that stuff off.
			caller.TryDestroy();
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
		else if(!m_DamageSourcesThisFrame.empty())
		{
			TakeDamage();
			if (m_IFrames > 0)
				m_IsInvulnerable = true;
		}

		if(!m_DamageSourcesThisFrame.empty())
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
		//Damageable& component = caller.GetComponent<Damageable>();
	}

	void Damageable::Draw(GameObject & selected)
	{
		if (!selected.HasCompoenent<Damageable>())
			return;
		Damageable& component = selected.GetComponent<Damageable>();

		ImGui::Text("Max Health");
		ImGui::SameLine();
		ImGui::DragInt("##maxHealth", &component.m_MaxHealth, 1.0f, 0);
		
		ImGui::Text("Invulnerability Frames");
		ImGui::SameLine();
		ImGui::DragInt("##maxIFrames", &component.m_IFrames, 1.0f, 0);

		ImGui::SeparatorText("Tags");
		
		if (ImGui::Button("Add Tag"))
		{
			component.m_TagsThatDamageThis.push_back("");
		}
		std::string labelBase = "Tag ";
		
		std::vector<size_t> indexesToRemove;
		for (size_t i = 0; i < component.m_TagsThatDamageThis.size(); i++)
		{
			ImGui::PushID(i);
			std::string tag = component.m_TagsThatDamageThis[i];
			ImGuiPanel::DrawStringProperty(labelBase + std::to_string(i), component.m_TagsThatDamageThis[i], tag);
			ImGui::SameLine();
			if (ImGui::Button("RemoveTag"))
			{
				indexesToRemove.push_back(i);
			}
			ImGui::PopID();
		}

		for (size_t index : indexesToRemove)
		{
			if (index < 0 || index >= component.m_TagsThatDamageThis.size())
				continue;

			if (index != component.m_TagsThatDamageThis.size() - 1)
			{
				for (size_t i = index; i < component.m_TagsThatDamageThis.size() - 1; i++)
				{
					component.m_TagsThatDamageThis.at(i) = component.m_TagsThatDamageThis.at(i + 1);
				}
			}

			component.m_TagsThatDamageThis.pop_back();
		}
	}

	void Damageable::Serialize(StreamWriter * stream, const GameObject & sourceObject)
	{
		Damageable& component = sourceObject.GetComponent<Damageable>();
		stream->WriteString(m_Name);
		stream->WriteArray<std::string>(component.m_TagsThatDamageThis);
		stream->WriteRaw(component.m_MaxHealth);
		stream->WriteRaw(component.m_IFrames);
	}

	bool Damageable::Deserialize(StreamReader * stream, GameObject & destinationObject)
	{
		Damageable& component = destinationObject.GetComponent<Damageable>();
		std::string name;
		stream->ReadString(name);
		if (name != m_Name)
			return false;

		stream->ReadArray<std::string>(component.m_TagsThatDamageThis);
		stream->ReadRaw(component.m_MaxHealth);
		stream->ReadRaw(component.m_IFrames);
		return true;
	}
	void Damageable::Reset()
	{
		m_CurrentHealth = m_MaxHealth;

		m_AccumulatedIFrames = 0;
		m_IsInvulnerable = false;

		if (!m_DamageSourcesThisFrame.empty())
			m_DamageSourcesThisFrame.clear();
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