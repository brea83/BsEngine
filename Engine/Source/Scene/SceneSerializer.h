#pragma once
#include "Scene/Scene.h"
namespace Pixie
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		//ToDo: implement human readable serialization for collaborative works
		void SerializeYaml(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeYaml(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

	private:
		Scene* m_Scene;
	};
}