#include "BsPrecompileHeader.h"
#include "GUID.h"
#include <random>

namespace Pixie
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandomNumEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	GUID::GUID()
		: m_GUID(s_UniformDistribution(s_RandomNumEngine))
	{}
	GUID::GUID(uint64_t guid)
		: m_GUID(guid)
	{}
	void GUID::Serialize(StreamWriter * fileWriter, const GUID & object)
	{
		fileWriter->WriteRaw<uint64_t>(object.m_GUID);
	}
	bool GUID::Deserialize(StreamReader * fileReader, GUID & object)
	{
		fileReader->ReadRaw<uint64_t>(object.m_GUID);
		return true;
	}
}