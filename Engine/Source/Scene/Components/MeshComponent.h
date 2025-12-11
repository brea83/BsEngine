#pragma once
#include "Core.h"
#include "BsPrecompileHeader.h"
//#include "Graphics/Primitives/Renderable.h"
#include "Scene/Components/Component.h"
#include "Graphics/Primitives/Mesh.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Pixie
{
	//class Texture;
	class GameObject;

	class MeshComponent : public Component
	{
	public:
		MeshComponent(/*GameObject* parent*/);
		//MeshComponent(/*GameObject* parent,*/ PrimitiveMeshType primitiveMesh);
		MeshComponent(PrimitiveMeshType primitiveMesh);
		MeshComponent(/*GameObject* parent,*/ const std::string& modelFilePath, const std::string& textureFilePath = "../Assets/Textures/Viking_House.png");
		~MeshComponent();

		bool Reload();

		// Inherited via Component
		void Initialize() ;
		void CleanUp() ;
		std::shared_ptr<Component> Clone() ;

		const std::string& Name() const  override{ return m_Name; }
		void SetName(const std::string& name)  { m_Name = name; }
		//GameObject* GetParentObject() const  { return m_ParentObject; }
		//void SetParentObject(GameObject* newParent) ;

		void SetFilePath(const std::string& modelFilePath) { }
		std::string GetFilePath() { return m_FilePath; }

		bool HasTexture() { return (m_Texture != nullptr ) && (m_Texture->TextureObject != 0); }

		void OnUpdate() ;
		
		void Render(Shader& currentShader);

		void Render(Shader& currentShader, Transform& transform);

	protected:
		//properties
		std::string m_Name{ "Mesh Component" };
		//GameObject* m_ParentObject{ nullptr };
		std::string m_FilePath{""};
		std::string m_TexturePath{ "" };
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		//Todo: replace textures with materials.
		std::shared_ptr<Texture> m_Texture{nullptr};

		bool LoadObj(const std::string& filePath, const std::string& textureFileName = "");

		
		//methods
		void LoadMeshAssimp(const std::string& filePath);

		//TODO: refactor this to look for matching models and meshes in the assetloader
		// and move the actual loading and processing to the asset loader
		// for now just following tutorial to see if I can get importing to work at all
		void ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<Transform> localTransform, aiNode* parentNode);
		aiMatrix4x4 CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode);
		void ProcessNode(aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices);
		std::shared_ptr<Mesh>  processMesh(aiMesh* mesh, const aiScene* assimpScene);
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType);

		friend class DetailsViewPanel;
	};

	// what does a model component need to know to tell the renderer where to render its meshes?
	// how many meshes it has
	// a ref to each unique mesh (data about verts in object space)
	// a way to track and adjust transform offsets from the model root instead of the world root

	//easy for single mesh. model just uses the game object's transform
}


