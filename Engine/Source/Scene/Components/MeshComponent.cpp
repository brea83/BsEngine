#include "BsPrecompileHeader.h"
#include "MeshComponent.h"
#include "Resources/AssetLoader.h"
#include "Scene/Components/Transform.h"
#include "Graphics/Primitives/Mesh.h"
#include <glm/glm.hpp>
#include "Assimp/AssimpGlmHelpers.h"
#include <filesystem>
#include <fstream>
#include "Scene/GameObject.h"
#include "EngineContext.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


namespace Pixie
{
	MeshComponent::MeshComponent()
 : m_Name("Model Component"), m_FilePath("")
	{ }

	MeshComponent::MeshComponent(PrimitiveMeshType primitiveMesh)
	: m_Name("Model Component")
	{
		std::shared_ptr<Mesh> mesh = AssetLoader::LoadPrimitive(primitiveMesh);
		if (mesh != nullptr)
		{
			m_Meshes.push_back(mesh);

			switch (primitiveMesh)
			{
			case PrimitiveMeshType::Triangle:
				m_FilePath = "PrimitiveMesh_Triangle";
				break;
			case PrimitiveMeshType::Quad:
				m_FilePath = "PrimitiveMesh_Quad";
				break;
			case PrimitiveMeshType::Cube:
				m_FilePath = "PrimitiveMesh_Cube";
				break;
			default:
				break;
			}

			m_Meshes.push_back(mesh);
		}
		else
		{
			m_FilePath = "";
		}
	}

	MeshComponent::MeshComponent( const std::string& modelFilePath, const std::string& textureFilePath)
		: m_Name("Mesh Component"), m_FilePath(modelFilePath)
	{
		m_MaterialInstance.BaseMapPath = textureFilePath;
		Reload();
	}


	bool MeshComponent::Reload()
	{
		std::string fileExtension = m_FilePath.substr(m_FilePath.find_last_of('.'));
		if (fileExtension == ".fbx")
		{
			LoadMeshAssimp(m_FilePath);
			return false;
		}
		else if (fileExtension == ".obj")
		{
			return LoadObj(m_FilePath, m_MaterialInstance.BaseMapPath);
		}
		return false;
	}

	MeshComponent::~MeshComponent()
	{
		std::cout << "DELETING " << m_Name << std::endl;
	}

	void MeshComponent::LoadMeshAssimp(const std::string & filePath)
	{
		// TODO: Refactor FBX loading
		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace);

		if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
		std::cout << "IMPORTING MESH " << m_Name << std::endl;
		std::cout << " DIRECTORY: " << m_FilePath << std::endl;
		/*for (unsigned int i = 0; i < assimpScene->mNumMaterials; i++)
		{
			aiMaterial* material = assimpScene->mMaterials[i];
			aiString name =  material->GetName();

			int diffuse = material->GetTextureCount(aiTextureType_DIFFUSE);
			int baseColor = material->GetTextureCount(aiTextureType_BASE_COLOR);
			int ambient = material->GetTextureCount(aiTextureType_AMBIENT);
			int ao = material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
			int metallic = material->GetTextureCount(aiTextureType_METALNESS);
			int normal = material->GetTextureCount(aiTextureType_NORMALS);
			int diffuseRoughness = material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
			int gltfRoughness = material->GetTextureCount(aiTextureType_GLTF_METALLIC_ROUGHNESS);
			int other = material->GetTextureCount(aiTextureType_UNKNOWN);

			std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
			std::cout << name.C_Str() << std::endl;
			std::cout << "--------" << std::endl;
			std::cout << "found " << diffuse << " diffuse type textures" << std::endl;
			std::cout << "found " << baseColor << " base color type textures" << std::endl;
			std::cout << "found " << ambient << " ambient type textures" << std::endl;
			std::cout << "found " << ao << " ambient occlusion type textures" << std::endl;
			std::cout << "found " << metallic << " metallic type textures" << std::endl;
			std::cout << "found " << normal << " normal type textures" << std::endl;
			std::cout << "found " << diffuseRoughness << " diffuse roughness type textures" << std::endl;
			std::cout << "found " << gltfRoughness << " gltf metallic roughness type textures" << std::endl;
			std::cout << "found " << other << " unknown type textures" << std::endl;
		}*/

		//std::cout << "====================================" << std::endl;
		//std::cout << "Reserving " << assimpScene->mNumMeshes << ", in m_Meshes" << std::endl;
		//std::cout << "====================================" << std::endl;
		m_Meshes.reserve(assimpScene->mNumMeshes);
		ProcessNode(assimpScene->mRootNode, assimpScene, aiMatrix4x4());
		return;
	}

	bool MeshComponent::LoadObj(const std::string& filePath, const std::string& textureFileName)
	{
		std::shared_ptr<Mesh> mesh = AssetLoader::LoadMesh(filePath);
		if (mesh == nullptr)
		{
			return false;
		}
		m_Meshes.clear();
		m_Meshes.push_back(mesh);

		if (textureFileName != "")
		{
			//m_Texture = AssetLoader::LoadTexture(m_TexturePath);
			//if (m_Texture == nullptr) return false;
			m_MaterialInstance.BaseMapPath = textureFileName;
			m_MaterialInstance.BaseMap = AssetLoader::LoadTexture(textureFileName);
			if (m_MaterialInstance.BaseMap == nullptr) return false;
		}

		return true;
	}

	void MeshComponent::ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<TransformComponent> localTransform, aiNode* parentNode)
	{

		if (parentNode) 
		{ 
			//localTransform->ParentTransform = m_ParentObject->GetTransform();
		}
		
		aiVector3D scaling;
		aiVector3D rotation;
		aiVector3D position;
		nodeMatrix.Decompose(scaling, rotation, position);

		localTransform->SetPosition(AssimpGlmHelpers::GetGlmVec(position));
		localTransform->SetScale(AssimpGlmHelpers::GetGlmVec(scaling));
		localTransform->SetRotationEuler(AssimpGlmHelpers::GetGlmVec(rotation), AngleType::Radians);
	}

	aiMatrix4x4 MeshComponent::CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode)
	{
		if (parentNode)
		{
			return (CombineTransformsToRoot(parentNode->mParent, parentNode)) * childNode->mTransformation.Inverse();

			// ((rootTransform) * child1) * child2) * child3
		}

		return childNode->mTransformation.Inverse();
		
	}

	void MeshComponent::ProcessNode(aiNode * node, const aiScene * assimpScene, aiMatrix4x4 combinedParentMatrices)
	{
		// the recursion starts with an identity matrix in combined parent matrices so this should be fine
		aiMatrix4x4 nodeTransform = CombineTransformsToRoot(node->mParent, node); // node->mTransformation;//
		//if root node set model's transform
		if (!node->mParent)
		{
			//ProcessTransform(node->mTransformation, m_ParentObject->GetTransform(), nullptr);
			//std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
			//std::cout << "Set MODEL transform" << std::endl;
			//std::cout << "Position: " << _transform->GetPosition().x << ", " << _transform->GetPosition().y << ", " << _transform->GetPosition().z << std::endl;
			//std::cout << "Rotation: " << _transform->GetRotationEuler().x << ", " << _transform->GetRotationEuler().y << ", " << _transform->GetRotationEuler().z << std::endl;
			//std::cout << "Scale: " << _transform->GetScale().x << ", " << _transform->GetScale().y << ", " << _transform->GetScale().z << std::endl;
		}
		else
		{
			//nodeTransform = node->mParent->mTransformation.Inverse() * nodeTransform;
			
		}



		// current node meshes
		//Mesh* meshWithCurrentNodeTransform = nullptr;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aiMesh = assimpScene->mMeshes[node->mMeshes[i]];
			//std::cout << "------------------------------" << std::endl;
			std::shared_ptr<Mesh> mesh = processMesh(aiMesh, assimpScene);

			//std::cout << "Set Mesh transform for: " << node->mName.C_Str() << std::endl;
			//ProcessTransform(nodeTransform, m_ParentObject->GetTransform(), node->mParent);
			//std::cout << "Position: " << mesh.GetTransform()->GetPosition().x << ", " << mesh.GetTransform()->GetPosition().y << ", " << mesh.GetTransform()->GetPosition().z << std::endl;
			//std::cout << "Rotation: " << mesh.GetTransform()->GetRotationEuler().x << ", " << mesh.GetTransform()->GetRotationEuler().y << ", " << mesh.GetTransform()->GetRotationEuler().z << std::endl;
			//std::cout << "Scale: " << mesh.GetTransform()->GetScale().x << ", " << mesh.GetTransform()->GetScale().y << ", " << mesh.GetTransform()->GetScale().z << std::endl;
			//meshWithCurrentNodeTransform = &mesh;
			m_Meshes.push_back(mesh);
		}

		// process children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], assimpScene, nodeTransform);
		}
	}

	std::shared_ptr<Mesh>  MeshComponent::processMesh(aiMesh * mesh, const aiScene * assimpScene)
	{
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{

			Mesh::Vertex vertex;
			
			//process position 
			aiVector3D sourcePosition = mesh->mVertices[i];
			vertex.Position = glm::vec3(sourcePosition.x, sourcePosition.y, sourcePosition.z);

			// normals 
			aiVector3D sourceNormal = mesh->mNormals[i];
			vertex.Normal = glm::vec3(sourceNormal.x, sourceNormal.y, sourceNormal.z);

			// texture uvs
			if (mesh->mTextureCoords[0])
			{
				aiVector3D sourceUV = mesh->mTextureCoords[0][i];
				vertex.UV1 = glm::vec2(sourceUV.x, sourceUV.y);
			}
			else
			{
				vertex.UV1 = glm::vec2(0.0f, 0.0f);
			}

			// colors
			if (mesh->mColors[0])
			{
				aiColor4D* sourceColor = mesh->mColors[i];
				vertex.Color = glm::vec3(sourceColor->r, sourceColor->g, sourceColor->b);
			}
			else
			{
				vertex.Color = vertex.Normal;
			}

			vertices.emplace_back(vertex);
		}
		// process indices
		
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}
		// process materials

		aiMaterial* material = assimpScene->mMaterials[mesh->mMaterialIndex];

		if (material != nullptr)
		{

			std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, TextureType::Diffuse);

			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			//std::vector<Texture*> specularMaps = loadMaterialTextures(material,
			//	aiTextureType_SPECULAR, "texture_specular");

			//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return std::make_shared<Mesh>(/*_uid, */vertices, indices, /*textures,*/ mesh->mName.C_Str());
	}

	std::vector<std::shared_ptr<Texture>> MeshComponent::loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType)
	{
		std::vector<std::shared_ptr<Texture>> textures;
		for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString filePath;
			material->GetTexture(type, i, &filePath);
			
			/*std::string typeName = TextureTypeToString.at(bsTextureType);
			std::cout << "TEXTURE PATH: " << filePath.C_Str() << std::endl;
			std::cout << "TYPE: " << typeName << std::endl;*/
			std::shared_ptr<Texture> texture = AssetLoader::LoadTexture(filePath.C_Str());
			if (texture == nullptr) continue;

			texture->Type = bsTextureType;
			textures.emplace_back(texture);
		}
		return textures;
	}

	void MeshComponent::Initialize()
	{

	}

	void MeshComponent::CleanUp()
	{}


	void MeshComponent::OnUpdate()
	{}

	void MeshComponent::Render(Shader& currentShader)
	{
		if (m_MaterialInstance.BaseMap != nullptr)
		{
			m_MaterialInstance.BaseMap->Bind(0);
			currentShader.SetUniformInt("Texture1", 0);
		}
		if (m_MaterialInstance.MetallicMap != nullptr)
		{
			m_MaterialInstance.MetallicMap->Bind(1);
			currentShader.SetUniformInt("MetallicMap", 1);
			currentShader.SetUniformBool("material.bUseMetalicMap", true);
		}
		else
		{
			currentShader.SetUniformBool("material.bUseMetalicMap", false);
		}

		currentShader.SetUniformFloat("material.specularPower", m_MaterialInstance.SpecularPower);
		currentShader.SetUniformFloat("material.smoothness", m_MaterialInstance.Smoothness);
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Render(currentShader);
		}

		if (m_MaterialInstance.BaseMap != nullptr)
		{
			m_MaterialInstance.BaseMap->UnBind();
			//currentShader.SetUniformInt("Texture1", 0);
		}
		if (m_MaterialInstance.MetallicMap != nullptr)
		{
			m_MaterialInstance.MetallicMap->UnBind();
		}
	}

}
