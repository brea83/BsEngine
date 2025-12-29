#pragma once
#include <string>

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
}