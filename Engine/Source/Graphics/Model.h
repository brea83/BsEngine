#pragma once
#include "BsPrecompileHeader.h"
#include "Graphics/Primitives/Renderable.h"
#include "Graphics/Primitives/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Texture;

class Model : public Renderable
{
public:
	Model(const std::string& modelFilePath, const std::string& textureDirectoryPath = "Assets/Textures");

	

	// Inherited via Renderable
	void Render(Shader& currentShader) override;
protected:
	//properties
	std::vector<Mesh> _meshes;
	std::string _directory;
	std::string _texturesDirectory{"Assets/Textures"};

	
	//methods
	void Init() override;
	void LoadModel(const std::string& filePath);
	//TODO: refactor this to look for matching models and meshes in the assetloader
	// and move the actual loading and processing to the asset loader
	// for now just following tutorial to see if I can get importing to work at all
	void ProcessTransform(aiMatrix4x4 nodeMatrix, Transform* localTransform, aiNode* parentNode);
	aiMatrix4x4 CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode);
	void ProcessNode(aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices);
	Mesh processMesh(aiMesh* mesh, const aiScene* assimpScene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
};

