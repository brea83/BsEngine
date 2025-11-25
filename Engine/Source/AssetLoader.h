#pragma once
#include "BsPrecompileHeader.h"
#include "Resource.h"
#include "TextResource.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include "Graphics/Primitives/Mesh.h"

#include <assimp/scene.h>

class AssetLoader
{
public:
	const static int MimimumAvailableMb{ 2 };
	static bool IsMemoryAvailable(int minimumAvailableMb);

	static std::shared_ptr<TextResource> LoadTextFile(const std::string& filePath);
	static bool ReLoadTextFile(const std::string& filePath);
	static std::shared_ptr<Shader> LoadShader(const std::string& vertPath, const std::string& fragPath);
	static std::shared_ptr<Texture> LoadTexture(const std::string& filePath);

	//static 
	static std::shared_ptr<Mesh> LoadPrimitive(PrimitiveMeshType primitiveType);
	static std::shared_ptr<Mesh> LoadObj(const std::string& filePath, const std::string& textureFilePath = "");
	//static std::shared_ptr<Mesh> LoadMesh(std::string& objContentsString);
	//static std::shared_ptr<Model> LoadFbx(const std::string& filePath);

	static std::unordered_map<std::string, std::shared_ptr<Resource>>& GetResources() { return _resources; }
	static void CleanUp();
private:
	static std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;

	static void ParsePathString(const std::string& inPath, std::string& outPath);
	static std::shared_ptr<Texture> LoadTextureParsedPath(const std::string& filePath);



	//fbx Importing functions 

	//static std::shared_ptr<Mesh> LoadMesh(aiMesh& mesh, const aiScene& assimpScene);
	//static void ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<Transform> localTransform, aiNode* parentNode);
	//static aiMatrix4x4 CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode);
	//static void ProcessNode(aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices);
	//static std::shared_ptr<Mesh>  processMesh(aiMesh* mesh, const aiScene* assimpScene);
	//static std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType);
};
