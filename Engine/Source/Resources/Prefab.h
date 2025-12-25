#pragma once
#include "Resource.h"
#include "Scene/GameObject.h"
#include "Resources/FileStream.h"

namespace Pixie
{
	class Prefab : public Resource
	{
	public:
		Prefab() : Resource(ResourceType::Prefab), m_BaseObject(GameObject()) 
		{ }

		Prefab(GameObject& baseObject)
			: Resource(ResourceType::Prefab), m_BaseObject(baseObject)
		{
			NameComponent* nameComponent = baseObject.TryGetComponent<NameComponent>();
			std::string name;
			if (nameComponent) name = nameComponent->Name;
			else name = "Prefab";

			m_Path.replace_filename(name);

		}

		static void Serialize(StreamWriter* stream, const Prefab& prefab)
		{
			stream->WriteString("Prefab");
			stream->WriteRaw<int>(s_Version);

			stream->WriteObject(prefab.m_BaseObject);
		}

		static bool Deserialize(StreamReader* stream, Prefab& copy)
		{
			std::string pMetaType;
			stream->ReadString(pMetaType);

			if (pMetaType != "Prefab")
			{
				std::cout << "Pmeta file was not a prefab type, cannot deserialize as prefab" << std::endl;
				return false;
			}

			int version{ -1 };
			stream->ReadRaw<int>(version);

			if (version != s_Version)
			{
				if (version != s_Version)
				{
					std::cout << "Warning serialization version doesn't match." << std::endl;
					std::cout << "Pixie Engine Expects: " << s_Version << std::endl;
					std::cout << "Found: " << version << std::endl;
				}
			}

			stream->ReadObject(copy.m_BaseObject);
			return true;
		}

		GameObject GetBaseObject() { return m_BaseObject; };
		std::filesystem::path GetPath() const { return m_Path; }
	private:
		std::filesystem::path m_Path{ "../Assets/Prefabs/fileName.pmeta" };
		static const int s_Version{ 0 };
		GameObject m_BaseObject{};
	};

}