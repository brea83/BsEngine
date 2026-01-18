#pragma once
#include "Resource.h"
#include <filesystem>

namespace Pixie
{
	class TextResource : public Resource
	{
	public:
		//TextResource(std::string contents) 
		//	: Resource(ResourceType::TextFile), Text(std::move(contents)), FileName("")
		//{}

		TextResource(std::string contents, const std::string& fileName)
			: Resource(ResourceType::TextFile), Text(std::move(contents)), FileName(fileName)
		{}
		std::string Text;
		std::filesystem::path FileName;

		std::string GetNameString()
		{
			return FileName.filename().string();
		}
	};

}