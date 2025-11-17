#pragma once
#include "BsPrecompileHeader.h"
#include "Resource.h"
#include "TextResource.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Model.h"
#include "Graphics/Texture.h"

//class Resource;
//class Model;
//class Texture;
//class Shader;

class AssetLoader
{
public:
	static std::shared_ptr<TextResource> LoadTextFile(const std::string& filePath);
	static bool ReLoadTextFile(const std::string& filePath);
	static std::shared_ptr<Shader> LoadShader(const std::string& vertPath, const std::string& fragPath);
	static std::shared_ptr<Texture> LoadTexture(const std::string& filePath);

	//static Model* LoadModel(const std::string& modelPath);

	static std::unordered_map<std::string, std::shared_ptr<Resource>>& GetResources() { return _resources; }
	static void CleanUp();
private:
	static std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;

	static void ParsePathString(const std::string& inPath, std::string& outPath);
	static std::shared_ptr<Texture> LoadTextureParsedPath(const std::string& filePath);
};

