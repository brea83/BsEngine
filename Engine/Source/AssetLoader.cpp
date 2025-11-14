#include "BsPrecompileHeader.h"
#include "AssetLoader.h"
#include <fstream>
#include "StbImageWrapper.h"
#include "TextResource.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture.h"


std::unordered_map<std::string, Resource*> AssetLoader::_resources;

std::string AssetLoader::LoadTextFile(const std::string& filePath)
{
	if (_resources.find(filePath) != _resources.end())
	{
		return ((TextResource*)_resources.at(filePath))->Text;
	}

	// no stored text asset from that path so make one
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();

	TextResource* textSource = new TextResource(buffer.str());
	_resources.emplace( filePath, textSource );

	return buffer.str();
}

bool AssetLoader::ReLoadTextFile(const std::string& filePath)
{
	TextResource* oldText = nullptr;
	if (_resources.find(filePath) != _resources.end())
	{
		oldText = (TextResource*)_resources.at(filePath);
	}

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return false;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();

	if (oldText == nullptr)
	{
		// we found no existing text file before so make and add a new one
		TextResource* textSource = new TextResource(buffer.str());

		_resources.emplace(filePath, textSource);
		return true;
	}

	oldText->Text = buffer.str();
	return true;
}

Shader* AssetLoader::LoadShader(const std::string& vertPath, const std::string& fragPath)
{

	if (_resources.find(vertPath + "|" + fragPath) != _resources.end())
	{
		return (Shader*)_resources.at(vertPath + "|" + fragPath);
	}
	
	std::string vertexGlsl = LoadTextFile(vertPath);
	std::string fragmentGlsl = LoadTextFile(fragPath);


	// no stored shader made from those file paths, so make one
	Shader* shader = new Shader(vertPath, fragPath);
	if (shader->IsValid())
	{
		_resources.emplace( vertPath + "|" + fragPath, shader );
	}
	return shader;
}

Texture* AssetLoader::LoadTexture(const std::string& filePath)
{
	if (_resources.find(filePath) != _resources.end())
	{
		return (Texture*)_resources.at(filePath);
	}


	//prep filepath
	std::string relativePath = "";
	std::cout << ":::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
	std::cout << "trying to load file path: " << filePath << std::endl;
	if (filePath.substr(0, filePath.find_first_of('\\')) == "..")
	{
		relativePath = "Assets" + filePath.substr(filePath.find_first_of('\\'));
		std::cout << "PATH WAS RELATIVE" << std::endl;
		std::cout << "path modified to: " << relativePath << std::endl;
	}

	StbImageData data;
	if (relativePath != "")
	{
		StbImageWrapper::LoadImage(relativePath, data);
	}
	else
	{
		StbImageWrapper::LoadImage(filePath, data);
	}

	if (!data.BLoadSuccess) return nullptr;

	Texture* texture = new Texture(data);

	if (texture->TextureObject == 0) return nullptr;

	_resources.emplace(filePath, texture);
	return texture;
}
