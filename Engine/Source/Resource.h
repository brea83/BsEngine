#pragma once

enum class ResourceType
{
	TextFile,
	Shader,
	Texture,
	Renderable,
	Model,
};
class Resource
{
public:
	Resource(ResourceType type) : Type(type) { }
	virtual ~Resource();

	ResourceType Type;
};

