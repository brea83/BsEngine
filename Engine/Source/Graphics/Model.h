#pragma once
#include "BsPrecompileHeader.h"
//#include "Graphics/Primitives/Renderable.h"
#include "Component.h"
#include "Graphics/Primitives/Mesh.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//class Texture;

class Model : public Component
{
public:
	Model(GameObject* parent, const std::string& modelFilePath, const std::string& textureFilePath = "Assets/Textures/Viking_House.png");
	~Model();

	void SetFilePath(const std::string& modelFilePath) { }
	std::string GetFilePath() { return _filePath; }

	bool Reload();

	// Inherited via Renderable
	void Render(Shader& currentShader);

protected:
	//properties
	std::vector<std::shared_ptr<Mesh>> _meshes;
	std::string _filePath;
	std::string _texturePath;
	bool LoadObj(const std::string& filePath, const std::string& textureFileName = "");

	
	//methods
	void LoadModelAssimp(const std::string& filePath);
	//TODO: refactor this to look for matching models and meshes in the assetloader
	// and move the actual loading and processing to the asset loader
	// for now just following tutorial to see if I can get importing to work at all
	void ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<Transform> localTransform, aiNode* parentNode);
	aiMatrix4x4 CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode);
	void ProcessNode(aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices);
	std::shared_ptr<Mesh>  processMesh(aiMesh* mesh, const aiScene* assimpScene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType);

	// Inherited via Component
	void Initialize() override;
	void CleanUp() override;
	std::shared_ptr<Component> Clone() override;
	void SetParentObject(GameObject* newParent) override;
	void OnUpdate() override;

	friend class DetailsViewPanel;
};

// what does a model component need to know to tell the renderer where to render its meshes?
// how many meshes it has
// a ref to each unique mesh (data about verts in object space)
// a way to track and adjust transform offsets from the model root instead of the world root

//easy for single mesh. model just uses the game object's transform


