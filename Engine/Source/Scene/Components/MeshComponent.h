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

	class MeshComponent
	{
	public:
		using StdPath = std::filesystem::path;
		MeshComponent();
		MeshComponent(PrimitiveMeshType primitiveMesh);
		MeshComponent(const std::string& modelFilePath, const std::string& textureFilePath = "");
		~MeshComponent();

		static constexpr SerializableComponentID ID{ SerializableComponentID::MeshComponent };
		bool Reload();

		void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; }
		

		const std::string& Name() const { return m_Name; }
		void SetName(const std::string& name)  { m_Name = name; }

		void SetFilePath(const std::string& path) { m_FilePath = path; }
		std::string GetFilePath() { return m_FilePath; }

		MaterialInstance& GetMaterialInstance() { return m_MaterialInstance; }

		bool HasTexture() { return (m_MaterialInstance.BaseMap != nullptr ) && (m_MaterialInstance.BaseMap->TextureObject != 0); }

		void OnUpdate() ;
		
		void Render(Shader& currentShader);

		static void Serialize(StreamWriter* stream, const MeshComponent& component)
		{
			stream->WriteString(component.m_Name);
			stream->WriteString(component.m_FilePath);
			stream->WriteObject<MaterialInstance>(component.m_MaterialInstance);

		}
		static bool Deserialize(StreamReader* stream, MeshComponent& component)
		{
			stream->ReadString(component.m_Name);

			std::string oldPath = component.m_FilePath;
			stream->ReadString(component.m_FilePath);

			std::string oldTexture = component.m_MaterialInstance.BaseMapPath;
			std::string oldMetalPath = component.m_MaterialInstance.MetallicMapPath;
			stream->ReadObject<MaterialInstance>(component.m_MaterialInstance);

			if (oldPath != component.m_FilePath 
				|| oldTexture != component.m_MaterialInstance.BaseMapPath
				|| oldMetalPath != component.m_MaterialInstance.MetallicMapPath)
			{
				component.Reload();
			}
			return true;
		}

		bool operator== (const MeshComponent& other) const
		{
			return other.m_Name == m_Name
				&& other.m_FilePath == m_FilePath
				&& other.m_Mesh == m_Mesh
				&& other.m_MaterialInstance == m_MaterialInstance;
		}

		bool operator!=(const MeshComponent& other) const
		{
			return !(*this == other);
		}

	protected:
		//properties
		std::string m_Name{ "Mesh Component" };
		std::string m_FilePath{""};
		

		std::shared_ptr<Mesh> m_Mesh;
		MaterialInstance m_MaterialInstance{};


		bool LoadObj(const StdPath& filePath, const std::string& textureFileName = "");

		
		//methods
		// ToDo: Refactor FBX loading
		//bool LoadFbx(GameObject& rootObject, const StdPath& filePath);
		
		//void ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<TransformComponent> localTransform, aiNode* parentNode);
		//aiMatrix4x4 CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode);
		//void ProcessNode(aiNode* node, const aiScene* assimpScene, aiMatrix4x4 combinedParentMatrices);
		//std::shared_ptr<Mesh>  processMesh(aiMesh* mesh, const aiScene* assimpScene);
		//std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType);

		friend class DetailsViewPanel;
	};

	// what does a model component need to know to tell the renderer where to render its meshes?
	// how many meshes it has
	// a ref to each unique mesh (data about verts in object space)
	// a way to track and adjust transform offsets from the model root instead of the world root

	//easy for single mesh. model just uses the game object's transform
}


