#pragma once
#include <string>
#include <map>
#include <unordered_map>

namespace Pixie
{
	class StreamReader
	{
	public:
		virtual ~StreamReader() = default;

		virtual bool IsStreamGood() const = 0;

		virtual uint64_t GetStreamPosition() = 0;
		virtual void SetStreamPosition(uint64_t position) = 0;

		virtual bool ReadData(char* destination, size_t size) = 0;

		operator bool() const { return IsStreamGood(); }

		bool ReadString(std::string& string);

		template<typename T>
		bool ReadRaw(T& type)
		{
			bool success = ReadData((char*)&type, sizeof(T));
			return success;
		}

		template<typename T>
		void ReadObject(T& object)
		{
			T::Deserialize(this, object);
		}

		//template<typename Key, typename Value>
	};
}