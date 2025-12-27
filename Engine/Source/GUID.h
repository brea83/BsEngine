#pragma once
#include <xhash>
#include <Resources/FileStream.h>
namespace Pixie
{
	class GUID
	{
	public:
		GUID();
		GUID(uint64_t guid);
		GUID(const GUID&) = default;

		operator uint64_t() const { return m_GUID; }

		static void Serialize(StreamWriter* fileWriter, const GUID& object);
		static bool Deserialize(StreamReader* fileReader, GUID& object);
	private:
		uint64_t m_GUID;
	};
}

namespace std
{
	template<>
	struct hash<Pixie::GUID>
	{
		std::size_t operator()(const Pixie::GUID& guid) const
		{
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}