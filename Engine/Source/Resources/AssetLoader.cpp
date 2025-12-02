#include "BsPrecompileHeader.h"
#include "AssetLoader.h"
#include <fstream>
#include "StbImageWrapper.h"
#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef LoadImage

#include "Graphics/Primitives/Triangle.h"
#include "Graphics/Primitives/QuadMesh.h"
#include "Graphics/Primitives/Cube.h"

#include <chrono>

#include "Assimp/AssimpGlmHelpers.h"

const static std::string s_SerializationVersion = "Version 0.1 of Bs Engine Serialization";
std::unordered_map<std::string, std::shared_ptr<Resource>> AssetLoader::_resources;

bool AssetLoader::IsMemoryAvailable(int minimumAvailableMb)
{
	MEMORYSTATUSEX statusEx;
	statusEx.dwLength = sizeof(statusEx);
	GlobalMemoryStatusEx(&statusEx);
	//std::cout << "********************" << std::endl;
	//std::cout << "Physical Memory" << std::endl;
	//std::cout << "Total: " << statusEx.ullTotalPhys / (1024 * 1024 ) << std::endl;
	//std::cout << "Available: " << statusEx.ullAvailPhys / (1024 * 1024) << std::endl;

	bool result = minimumAvailableMb < statusEx.ullAvailPhys / (1024 * 1024);
	//std::cout << "Available memory > minimum required == " << result << std::endl;
	return result;
}

std::shared_ptr<TextResource> AssetLoader::LoadTextFile(const std::string& filePath)
{
	if (!IsMemoryAvailable(MimimumAvailableMb))
	{
		std::cout << "LOAD TEXT FILE ERROR: available memory less than " << std::endl;
		return nullptr;
	}

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
	if (!IsMemoryAvailable(MimimumAvailableMb))
	{
		std::cout << "LOAD SHADER FILE ERROR: available memory less than " << std::endl;
		return nullptr;
	}

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
	if (!IsMemoryAvailable(MimimumAvailableMb))
	{
		std::cout << "LOAD TEXTURE FILE ERROR: available memory less than " << std::endl;
		return nullptr;
	}

	//prep filepath
	std::string relativePath = "";
	ParsePathString(filePath, relativePath);

	if (relativePath != "" && relativePath != filePath)
	{
		return LoadTextureParsedPath(relativePath);
	}
	
	return LoadTextureParsedPath(filePath);
}



std::shared_ptr<Mesh> AssetLoader::LoadMesh(const std::string& filePath)
{
	std::string serializedPath = CheckForSerializedVersion(filePath);
	if ( serializedPath != "")
	{
		auto timerStart = std::chrono::high_resolution_clock::now();
		std::shared_ptr<Mesh> mesh = LoadSerializedMesh(serializedPath);
		auto timerEnd = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart);
		std::cout << "Loading serialized mesh took: " << duration.count() << " microseconds." << std::endl;

		return mesh;
	}

	std::shared_ptr<Mesh> mesh;
	std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
	if (fileExtension == ".fbx")
	{
		//LoadModelAssimp(filePath);
	}
	else if (fileExtension == ".obj")
	{
		auto timerStart = std::chrono::high_resolution_clock::now();
		mesh = LoadObj(filePath);
		auto timerEnd = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart);
		std::cout << "Loading Obj took: " << duration.count() << " microseconds." << std::endl;
	}
	
	return mesh;
}

std::shared_ptr<Mesh> AssetLoader::LoadPrimitive(PrimitiveMeshType primitiveType)
{
	switch (primitiveType)
	{
	case PrimitiveMeshType::Triangle:
	{
		if (_resources.find("PrimitiveMesh_Triangle") != _resources.end())
		{
			auto resourcePtr = std::dynamic_pointer_cast<Mesh>(_resources.at("PrimitiveMesh_Triangle"));
			if (resourcePtr) return resourcePtr;
		}
		std::shared_ptr<Mesh> mesh = std::make_shared<Triangle>();

		_resources.emplace("PrimitiveMesh_Triangle", mesh);
		return mesh;
	}
	case PrimitiveMeshType::Quad:
	{
		if (_resources.find("PrimitiveMesh_Quad") != _resources.end())
		{
			auto resourcePtr = std::dynamic_pointer_cast<Mesh>(_resources.at("PrimitiveMesh_Quad"));
			if (resourcePtr) return resourcePtr;
		}
		std::shared_ptr<Mesh> mesh = std::make_shared<Quad>();

		_resources.emplace("PrimitiveMesh_Quad", mesh);
		return mesh;
	}
	case PrimitiveMeshType::Cube:
	{
		if (_resources.find("PrimitiveMesh_Cube") != _resources.end())
		{
			auto resourcePtr = std::dynamic_pointer_cast<Mesh>(_resources.at("PrimitiveMesh_Cube"));
			if (resourcePtr) return resourcePtr;
		}
		std::shared_ptr<Mesh> mesh = std::make_shared<Cube>();

		_resources.emplace("PrimitiveMesh_Cube", mesh);
		return mesh;
	}
	default:
		break;
	}
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<Mesh> AssetLoader::LoadObj(const std::string& filePath, const std::string& textureFilePath)
{
	if (!IsMemoryAvailable(MimimumAvailableMb))
	{
		std::cout << "LOAD OBJ FILE ERROR: available memory less than " << std::endl;
		return nullptr;
	}

	if (_resources.find(filePath) != _resources.end())
	{
		auto resourcePtr = std::dynamic_pointer_cast<Mesh>(_resources.at(filePath));
		if (resourcePtr) return resourcePtr;
	}

	std::ifstream file(filePath);

	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return nullptr;
	}
	std::string name = filePath.substr(filePath.find_last_of('/') + 1, filePath.find_last_of("."));

	std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
	std::cout << "IMPORTING OBJ " << name << std::endl;
	std::cout << " FilePath: " << filePath << std::endl;
	std::string line;

	std::vector<Mesh::Vertex> vertices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	while (std::getline(file, line))
	{
		std::istringstream lineStream(line);
		std::string firstWord;
		lineStream >> firstWord;

		if (firstWord == "v")
		{
			// do vertex stuff
			glm::vec3 position;
			lineStream >> position.x >> position.y >> position.z;
			positions.push_back(position);
		}

		if (firstWord == "vt")
		{
			// texture coordinates
			glm::vec2 uv;
			lineStream >> uv.x >> uv.y;
			uvs.push_back(uv);
		}

		if (firstWord == "vn")
		{
			// normals
			glm::vec3 normal;
			lineStream >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}

		//if (firstWord == "vp")
		//{
		//	// parameter?
		//}

		if (firstWord == "f")
		{
			// faces data
			// need to double check but I think all formal exports require faces to come After all the other data
			//this will probably not get us quite enough spaces reserved, but it is probably closer than the empty init
			if (vertices.capacity() < positions.size()) vertices.reserve(positions.size());

			// format vertex_index/texture_index/normal_index
			// -1 referring to the last element of vertex list.

			std::vector<Mesh::ObjPackedIndices> objIndices;
			std::string word;
			while (lineStream >> word)
			{
				std::istringstream wordStream(word);
				std::string positionString, uvString, normalString;

				int relativePosIndex;
				int relativeUvIndex;
				int relativeNormalIndex;

				if (std::getline(wordStream, positionString, '/'))
				{
					relativePosIndex = std::atoi(positionString.c_str());
					relativePosIndex = std::max(0, relativePosIndex - 1);
				}
				else
				{
					relativePosIndex = 0;
				}

				if (std::getline(wordStream, uvString, '/'))
				{
					relativeUvIndex = std::atoi(uvString.c_str());
					relativeUvIndex = std::max(0, relativeUvIndex - 1);
				}
				else
				{
					relativeUvIndex = relativePosIndex;
				}

				if (std::getline(wordStream, normalString, '/'))
				{
					relativeNormalIndex = std::atoi(normalString.c_str());
					relativeNormalIndex = std::max(0, relativeNormalIndex - 1);
				}
				else
				{
					relativeNormalIndex = relativePosIndex;
				}

				int positionIndex, normalIndex, uvIndex;
				positionIndex = (relativePosIndex >= 0) ? relativePosIndex : positions.size() + relativePosIndex;
				uvIndex = (relativeUvIndex >= 0) ? relativeUvIndex : uvs.size() + relativeUvIndex;
				normalIndex = (relativeNormalIndex >= 0) ? relativeNormalIndex : normals.size() + relativeNormalIndex;

				objIndices.push_back(Mesh::ObjPackedIndices{ positionIndex, uvIndex, normalIndex });
			}

			//triangulate assuming n >3-gons are convex and coplanar
			for (size_t i = 1; i + 1 < objIndices.size(); i++)
			{
				const Mesh::ObjPackedIndices* point[3] = { &objIndices[0], &objIndices[i], &objIndices[i + 1] };

				//https://wikis.khronos.org/opengl/Calculating_a_Surface_Normal
				// U and V are the vectors used to calculate surface normal
				// U is point2 - point1 V is point3 - point1
				// normal is U cross V

				glm::vec3 U(positions[point[1]->Position] - positions[point[0]->Position]);
				glm::vec3 V(positions[point[2]->Position] - positions[point[0]->Position]);
				glm::vec3 faceNormal = glm::normalize(glm::cross(U, V));

				// make the vertex for the mesh

				for (size_t j = 0; j < 3; j++)
				{
					Mesh::Vertex vertex;

					vertex.Position = positions[point[j]->Position];
					vertex.Normal = (point[j]->Normal != 0 && normals.size() > 0) ? normals[point[j]->Normal] : faceNormal;
					if (uvs.size() > 0) vertex.UV1 = uvs[point[j]->Uv];

					// check if identical vertex exists to use its index instead
					auto found = std::find(vertices.begin(), vertices.end(), vertex);

					if (found != vertices.end())
					{
						size_t index = distance(vertices.begin(), found);
						indices.push_back(index);
					}
					else
					{
						indices.push_back(vertices.size());
						vertices.push_back(vertex);
					}
				}
			}
		}
	}
	file.close();


	std::shared_ptr<Mesh> mesh;

	mesh = std::make_shared<Mesh>(vertices, indices, name);

	if (mesh == nullptr) return nullptr;

	std::string serializedPath = SerializeMesh(filePath, mesh);
	if (serializedPath != "")
	{
		_resources.emplace(serializedPath, mesh);
		return mesh;
	}

	_resources.emplace(filePath, mesh);
	return mesh;
}

std::shared_ptr<Mesh> AssetLoader::LoadFbx(const std::string& filePath, GameObject* rootObject)
{
	Assimp::Importer importer;
	const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace);
	
	if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
	std::cout << "IMPORTING " << filePath << std::endl;

	std::vector<Mesh> meshes;
	meshes.reserve(assimpScene->mNumMeshes);
	ProcessNode(meshes, assimpScene->mRootNode, assimpScene, aiMatrix4x4());
	return nullptr;
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
	}

	StbImageData data;
	StbImageWrapper::LoadImage(filePath, data);

	if (!data.BLoadSuccess) return nullptr;

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(data);

	if (texture->TextureObject == 0) return nullptr;

	_resources.emplace(filePath, texture);
	return texture;
}

std::string AssetLoader::CheckForSerializedVersion(const std::string& filePath)
{
	std::string serializedPath = filePath.substr(0, filePath.find_last_of('.'));
	serializedPath.append(".bmeta");

	if (_resources.find(serializedPath) != _resources.end()) return serializedPath;

	std::ifstream file(serializedPath);

	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << serializedPath << std::endl;
		return "";
	}
	file.close();
	return serializedPath;
}

std::string AssetLoader::SerializeMesh(const std::string& filePath, std::shared_ptr<Mesh> mesh)
{
	std::string serializedPath = filePath.substr(0, filePath.find_last_of('.'));
	serializedPath.append(".bmeta");

	std::ofstream file(serializedPath, std::ios_base::binary);
	if (!file.is_open())
	{
		std::cerr << "ERROR: failed to open file to serialize mesh: " << serializedPath << std::endl;
		return "";
	}

	//metadata about the mesh structure
	file.write((char*)s_SerializationVersion.c_str(), s_SerializationVersion.size() * sizeof(char));
	size_t numVertices = mesh->GetVertices().size();
	file.write((char*)&numVertices, sizeof(size_t));
	size_t numIndices = mesh->GetIndices().size();
	file.write((char*)&numIndices, sizeof(size_t));

	file.write((char*)mesh->GetVertices().data(), sizeof(Mesh::Vertex) * mesh->GetVertices().size());
	file.write((char*)mesh->GetIndices().data(), sizeof(unsigned int) * mesh->GetIndices().size());

	file.close();


	return serializedPath;
}

std::shared_ptr<Mesh> AssetLoader::LoadSerializedMesh(const std::string& filePath)
{
	if (_resources.find(filePath) != _resources.end())
	{
		auto resourcePtr = std::dynamic_pointer_cast<Mesh>(_resources.at(filePath));
		if (resourcePtr) return resourcePtr;
	}

	std::ifstream file(filePath, std::ios_base::binary);
	if (!file.is_open())
	{
		return nullptr;
	}

	std::string version;
	version.resize(s_SerializationVersion.size());
	file.read((char*)version.data(), s_SerializationVersion.size() * sizeof(char));

	size_t numVertices;
	file.read((char*)&numVertices, sizeof(size_t));
	if (numVertices > 100000000) throw std::runtime_error("Error reading serialized mesh, too many vertices");

	size_t numIndices;
	file.read((char*)&numIndices, sizeof(size_t));
	if (numIndices > 300000000) throw std::runtime_error("Error reading serialized mesh, too many indices");

	std::vector<Mesh::Vertex> vertices;
	vertices.resize(numVertices);
	file.read((char*)vertices.data(), sizeof(Mesh::Vertex) * numVertices);

	std::vector<unsigned int> indices;
	indices.resize(numIndices);
	file.read((char*)indices.data(), sizeof(unsigned int) * numIndices);

	file.close();
	
	return std::make_shared<Mesh>(vertices, indices);
}

void AssetLoader::ProcessNode(std::vector<Mesh>& meshes, aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices)
{}
