#pragma once
#include "Scene/Scene.h"
#include "Resources/FileStream.h"

namespace Pixie
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		//ToDo: implement human readable serialization for collaborative works
		void Serialize(std::filesystem::path filePath);

		bool Deserialize(std::filesystem::path filePath);

	private:
		Scene* m_Scene;
	};
}