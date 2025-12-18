#include "BsPrecompileHeader.h"
#include "SceneSerializer.h"


namespace Pixie
{
	SceneSerializer::SceneSerializer(Scene* scene)
		: m_Scene(scene)
	{}

	void SceneSerializer::SerializeYaml(const std::string& filepath)
	{}
	void SceneSerializer::SerializeBinary(const std::string & filepath)
	{}

	bool SceneSerializer::DeserializeYaml(const std::string & filepath)
	{
		return false;
	}
	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		return false;
	}
}