#pragma once
#include "Resource.h"

namespace Pixie
{
	class TextResource : public Resource
	{
	public:
		TextResource(std::string contents) : Resource(ResourceType::TextFile), Text(std::move(contents)) {}
		std::string Text;
	};

}