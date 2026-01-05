#include "BsPrecompileHeader.h"
#include "Prefab.h"

namespace Pixie
{
	void Prefab::Serialize(StreamWriter* stream, const Prefab& prefab)
	{
		stream->WriteString("Prefab");
		stream->WriteRaw<int>(s_Version);

		stream->WriteObject(prefab.m_BaseObject);
	}

	bool Prefab::Deserialize(StreamReader* stream, Prefab& copy)
	{
		std::string pMetaType;
		stream->ReadString(pMetaType);

		if (pMetaType != "Prefab")
		{
			Logger::Log(LOG_ERROR, "Pmeta file was not a prefab type, cannot deserialize as prefab");
			return false;
		}

		int version{ -1 };
		stream->ReadRaw<int>(version);

		if (version != s_Version)
		{
			if (version != s_Version)
			{
				Logger::Log(LOG_WARNING, "Warning serialization version doesn't match.");
				Logger::Log(LOG_WARNING, "Pixie Engine Expects: {}", s_Version);
				Logger::Log(LOG_WARNING, "Found: {}", version);
			}
		}

		stream->ReadObject(copy.m_BaseObject);
		return true;
	}


}
