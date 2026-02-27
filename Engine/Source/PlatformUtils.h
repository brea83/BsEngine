#pragma once
#include <string>
#include <filesystem>

namespace Pixie
{
	class FileDialogs
	{
	public:
		// reminder the filter is which file extensions are valid in the file dialog
		// also returns empty string if canceled
		static std::string OpenFile(const char* filter);
		// reminder the filter is which file extensions are valid in the file dialog
		// also returns empty string if canceled
		static std::string SaveFile(const char* filter);
	};


	class PathParsing
	{
	public:
		// returns true if path was able to be shorted to a relative path based on the project's ../Assets/* folder structure
		// if a relative path was able to be constructed it is accessed through the outPath you provided
		// if no relative path was found then outPath will be set equal to inPath
		static bool IsPathProjectRelative(const std::filesystem::path& inPath, std::filesystem::path& outPath);
	};
}