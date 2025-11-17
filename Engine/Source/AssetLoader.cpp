#include "BsPrecompileHeader.h"
#include "AssetLoader.h"
#include <fstream>
#include "StbImageWrapper.h"
//#include "TextResource.h"
//#include "Graphics/Shaders/Shader.h"
//#include "Graphics/Texture.h"


std::unordered_map<std::string, std::shared_ptr<Resource>> AssetLoader::_resources;

std::shared_ptr<TextResource> AssetLoader::LoadTextFile(const std::string& filePath)
{
	if (_resources.find(filePath) != _resources.end())
	{
		auto textResourcePtr = std::dynamic_pointer_cast<TextResource>(_resources.at(filePath));
		if (textResourcePtr) return textResourcePtr;
	}

	// no stored text asset from that path so make one
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return nullptr;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();

	std::shared_ptr<TextResource> textSource = std::make_shared<TextResource>( TextResource(buffer.str()));
	_resources.emplace( filePath, textSource );

	return textSource;
}

bool AssetLoader::ReLoadTextFile(const std::string& filePath)
{
	bool foundText = false;

	if (_resources.find(filePath) != _resources.end())
	{
		foundText = true;
	}

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return false;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();

	if (foundText)
	{
		auto textResourcePtr = std::dynamic_pointer_cast<TextResource>(_resources.at(filePath));
		if (textResourcePtr)
		{
			textResourcePtr->Text = buffer.str();
			return true;
		}
	}
	else
	{
		// we found no existing text file before so make and add a new one
		TextResource* textSource = new TextResource(buffer.str());

		_resources.emplace(filePath, textSource);
		return true;
	}
	std::cout << "ERROR, FILE FOUND IN ASSETS, BUT COULD NOT BE ACCESSED AS A TextResource.cpp" << std::endl;
	return false;
}

std::shared_ptr<Shader> AssetLoader::LoadShader(const std::string& vertPath, const std::string& fragPath)
{

	if (_resources.find(vertPath + "|" + fragPath) != _resources.end())
	{
		auto shader = std::dynamic_pointer_cast<Shader>(_resources.at(vertPath + "|" + fragPath));
		if (shader) return shader;
		//return (Shader*)_resources.at(vertPath + "|" + fragPath);
	}
	
	std::string vertexGlsl = LoadTextFile(vertPath)->Text;
	std::string fragmentGlsl = LoadTextFile(fragPath)->Text;


	// no stored shader made from those file paths, so make one
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertPath, fragPath);

	if (shader->IsValid())
	{
		_resources.emplace( vertPath + "|" + fragPath, shader );
	}
	return shader;
}

std::shared_ptr<Texture> AssetLoader::LoadTexture(const std::string& filePath)
{
	//prep filepath
	std::string relativePath = "";
	ParsePathString(filePath, relativePath);

	if (relativePath != "" && relativePath != filePath)
	{
		return LoadTextureParsedPath(relativePath);
	}
	
	return LoadTextureParsedPath(filePath);
}

void AssetLoader::CleanUp()
{
	std::vector<std::string> resourcesToDelete;
	for (auto pair : _resources)
	{
		int useCount = pair.second.use_count();
		//std::cout << pair.first <<", has " << std::to_string(useCount) << " live pointers" << std::endl;
		if( useCount == 2)
		{
			// two uses means one is this for loop and the other is the instance in _resources ie: nothing outside the loader is using it
			resourcesToDelete.emplace_back(pair.first);
		}
	}

	for (std::string key : resourcesToDelete)
	{
		_resources.erase(key);
	}
}

void AssetLoader::ParsePathString(const std::string& inPath, std::string& outPath)
{
	//prep filepath
	
	if (inPath.substr(0, inPath.find_first_of('\\')) == "..")
	{
		outPath = "Assets" + inPath.substr(inPath.find_first_of('\\'));
		//std::cout << "PATH WAS RELATIVE, AND STARTED WITH .." << std::endl;
		//std::cout << "path modified to: " << outPath << std::endl;
		return;
	}

	outPath = inPath;
}

std::shared_ptr<Texture> AssetLoader::LoadTextureParsedPath(const std::string& filePath)
{
	if (_resources.find(filePath) != _resources.end())
	{
		auto texture = std::dynamic_pointer_cast<Texture>(_resources.at(filePath));
		if (texture) return texture;
		//return (Texture*)_resources.at(filePath);
	}

	StbImageData data;
	StbImageWrapper::LoadImage(filePath, data);

	if (!data.BLoadSuccess) return nullptr;

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(data);

	if (texture->TextureObject == 0) return nullptr;

	_resources.emplace(filePath, texture);
	return texture;
}
