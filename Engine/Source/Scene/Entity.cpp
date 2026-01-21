#include "BsPrecompileHeader.h"
#include "Entity.h"

namespace Pixie
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{}

	const std::string& Entity::GetName() const
	{
		return GetComponent<NameComponent>().Name;
	}

	void Entity::WarnSceneNull(entt::entity entityHandle, const std::string& attemptedAction)
	{
		Logger::Log(LOG_WARNING, "Entity with entt handle: {}, tried to {}, but m_Scene is NULLPTR", (uint32_t)entityHandle, attemptedAction);
	}

	void Entity::WarnEntityInvalid(const std::string& attemptedAction)
	{
		Logger::Log(LOG_WARNING, "Invalid Entity tried to: {}", attemptedAction);
	}
}
