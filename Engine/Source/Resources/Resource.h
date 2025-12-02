#pragma once

enum class ResourceType
{
	TextFile,
	Shader,
	Texture,
	Renderable,
	Mesh,
};
class Resource
{
public:
	Resource(ResourceType type) : Type(type) { }
	virtual ~Resource();

	ResourceType Type;
};

