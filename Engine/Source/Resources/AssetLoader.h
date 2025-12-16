#pragma once
#include "BsPrecompileHeader.h"
#include "Resource.h"
#include "TextResource.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture.h"
#include "Scene/Components/MeshComponent.h"
#include "Graphics/Primitives/Mesh.h"

#include <assimp/scene.h>
namespace Pixie
{
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
		static std::shared_ptr<Mesh>LoadMesh(const std::string& filePath);
		static std::shared_ptr<Mesh> LoadPrimitive(PrimitiveMeshType primitiveType);
		static std::shared_ptr<Mesh> LoadObj(const std::string& filePath, const std::string& textureFilePath = "");
		//static std::shared_ptr<Mesh> LoadMesh(std::string& objContentsString);
		static std::shared_ptr <Mesh> LoadFbx(const std::string& filePath, GameObject* rootObject);

		static std::unordered_map<std::string, std::shared_ptr<Resource>>& GetResources() { return _resources; }
		static void CleanUp();
	private:
		static std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;

		static void ParsePathString(const std::string& inPath, std::string& outPath);
		static std::shared_ptr<Texture> LoadTextureParsedPath(const std::string& filePath);

		static std::string CheckForSerializedVersion(const std::string& filePath);
		static std::string SerializeMesh(const std::string& filePath, std::shared_ptr<Mesh> mesh);
		//static std::string SerializeAssimpSubMesh(const std::string& filePath, const std::string& meshName, std::shared_ptr<Mesh> mesh);
		static std::shared_ptr<Mesh> LoadSerializedMesh(const std::string& filePath);

		//fbx Importing functions 

		static void ProcessNode(std::vector<Mesh>& meshes, aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices);
		//static std::shared_ptr<Mesh> LoadMesh(aiMesh& mesh, const aiScene& assimpScene);
		//static void ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<TransformComponent> localTransform, aiNode* parentNode);
		//static aiMatrix4x4 CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode);
		//static std::shared_ptr<Mesh>  processMesh(aiMesh* mesh, const aiScene* assimpScene);
		//static std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType);
	};
}

//fbx loading plan
// use similar method as for serializing scene? but it's just the data to construct the fbx again
// first unpack saves meshes in loader with a lookup of their file | node name
//		it will also need to serialize the layout of entities and components used to build the prefab