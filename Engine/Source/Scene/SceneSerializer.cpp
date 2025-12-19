#include "BsPrecompileHeader.h"
#include "SceneSerializer.h"


namespace Pixie
{
	SceneSerializer::SceneSerializer(Scene* scene)
		: m_Scene(scene)
	{}


	void SceneSerializer::Serialize(std::filesystem::path filePath)
	{
		FileStreamWriter fileStream(filePath);
		// needs to serialize name, and serialize the registry
		fileStream.WriteString(m_Scene->m_Name);
		//get view of all entities in registry
		// make game objects of them, and do gameobject::serialize on them?

		auto view = m_Scene->m_Registry.view<entt::entity>();

		int entityCount = 0;
		if (view)
		{
			entityCount = view.size();
		}

		fileStream.WriteRaw<int>(entityCount);
	}


	bool SceneSerializer::Deserialize(std::filesystem::path filePath)
	{
		FileStreamReader fileStream(filePath);
		fileStream.ReadString(m_Scene->m_Name);

		int entityCount;
		fileStream.ReadRaw<int>(entityCount);
		return true;
	}
}