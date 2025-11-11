#pragma once
#include "Resource.h"
class TextResource : public Resource
{
public:
	TextResource(std::string contents): Resource(ResourceType::TextFile), Text(std::move(contents)) { }
	std::string Text;
};

