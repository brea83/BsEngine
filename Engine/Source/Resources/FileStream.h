#pragma once
#include "StreamWriter.h"
#include "StreamReader.h"

#include <filesystem>
#include <fstream>

namespace Pixie
{
	//---------------------------------------------------
	// FileStreamWriter
	class FileStreamWriter : public StreamWriter
	{
	public:
		FileStreamWriter(const std::filesystem::path& path);
		FileStreamWriter(const FileStreamWriter&) = delete;
		virtual ~FileStreamWriter();

		// Inherited via StreamWriter
		bool IsStreamGood() const override { return m_Stream.good(); }
		uint64_t GetStreamPosition() override { return m_Stream.tellp(); }
		void SetStreamPosition(uint64_t position) override { m_Stream.seekp(position); }
		bool WriteData(const char* data, size_t size) override;

	private:
		std::filesystem::path m_Path;
		std::ofstream m_Stream;
	};

	//---------------------------------------------------
	// FileStreamReader

	class FileStreamReader : public StreamReader
	{
	public:
		FileStreamReader(const std::filesystem::path& path);
		FileStreamReader(const FileStreamReader&) = delete;
		~FileStreamReader();


		// Inherited via StreamReader
		bool IsStreamGood() const override { return m_Stream.good(); }
		uint64_t GetStreamPosition() override { return m_Stream.tellg(); }
		void SetStreamPosition(uint64_t position) override { m_Stream.seekg(position); }
		bool ReadData(char* destination, size_t size) override;

	private:
		std::filesystem::path m_Path;
		std::ifstream m_Stream;
	};
}