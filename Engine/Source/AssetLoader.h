#pragma once
#include "BsPrecompileHeader.h"
#include "Resource.h"

//class Resource;
class Model;
class Texture;
class Shader;

class AssetLoader
{
public:
	static std::string LoadTextFile(const std::string& filePath);
	static bool ReLoadTextFile(const std::string& filePath);
	static Shader* LoadShader(const std::string& vertPath, const std::string& fragPath);
	static Texture* LoadTexture(const std::string& filePath);
	// TODO: MAKE MODEL CLASS
	//static Model* LoadModel(const std::string& modelPath);

	static std::unordered_map<std::string, Resource*>& GetResources() { return _resources; }
private:
	static std::unordered_map<std::string, Resource*> _resources;
};

