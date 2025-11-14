#include "BsPrecompileHeader.h"
#include "Model.h"
#include "Graphics/Texture.h"
#include "AssetLoader.h"
#include "Graphics/Primitives/Transform.h"
#include "Graphics/Primitives/Mesh.h"
#include <glm/glm.hpp>
#include "Assimp/AssimpGlmHelpers.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Model::Model(const std::string& modelFilePath, const std::string& textureDirectoryPath)
	: _texturesDirectory(textureDirectoryPath)
{
	_directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));
	std::cout << "IMPORTING MODEL WITH DIRECTORY: " << _directory << std::endl;

	LoadModel(modelFilePath);
}

void Model::Init()
{}

void Model::LoadModel(const std::string & filePath)
{
	Assimp::Importer importer;
	const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

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

	ProcessNode(assimpScene->mRootNode, assimpScene, aiMatrix4x4());
}

void Model::ProcessTransform(aiMatrix4x4 nodeMatrix, Transform* localTransform, aiNode* parentNode)
{

	if (parentNode) 
	{ 
		localTransform->ParentTransform = _transform; 
	}
	
	aiVector3D scaling;
	aiVector3D rotation;
	aiVector3D position;
	nodeMatrix.Decompose(scaling, rotation, position);

	localTransform->SetPosition(AssimpGlmHelpers::GetGlmVec(position));
	localTransform->SetScale(AssimpGlmHelpers::GetGlmVec(scaling));
	localTransform->SetRotationEuler(AssimpGlmHelpers::GetGlmVec(rotation), AngleType::Radians);
}

aiMatrix4x4 Model::CombineTransformsToRoot(aiNode* parentNode, aiNode* childNode)
{
	if (parentNode)
	{
		return (CombineTransformsToRoot(parentNode->mParent, parentNode)) * childNode->mTransformation.Inverse();

		// ((rootTransform) * child1) * child2) * child3
	}

	return childNode->mTransformation.Inverse();
	 
}

void Model::ProcessNode(aiNode * node, const aiScene * assimpScene, aiMatrix4x4 combinedParentMatrices)
{
	// the recursion starts with an identity matrix in combined parent matrices so this should be fine
	aiMatrix4x4 nodeTransform = CombineTransformsToRoot(node->mParent, node); // node->mTransformation;//
	//if root node set model's transform
	if (!node->mParent)
	{
		ProcessTransform(node->mTransformation, _transform, nullptr);
		std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
		std::cout << "Set MODEL transform" << std::endl;
		std::cout << "Position: " << _transform->GetPosition().x << ", " << _transform->GetPosition().y << ", " << _transform->GetPosition().z << std::endl;
		std::cout << "Rotation: " << _transform->GetRotationEuler().x << ", " << _transform->GetRotationEuler().y << ", " << _transform->GetRotationEuler().z << std::endl;
		std::cout << "Scale: " << _transform->GetScale().x << ", " << _transform->GetScale().y << ", " << _transform->GetScale().z << std::endl;
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
		Mesh mesh = processMesh(aiMesh, assimpScene);

		ProcessTransform(nodeTransform, mesh.GetTransform(), node->mParent);
		std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
		std::cout << "Set Mesh #" << node->mName.C_Str() << " transform" << std::endl;
		std::cout << "Position: " << mesh.GetTransform()->GetPosition().x << ", " << mesh.GetTransform()->GetPosition().y << ", " << mesh.GetTransform()->GetPosition().z << std::endl;
		std::cout << "Rotation: " << mesh.GetTransform()->GetRotationEuler().x << ", " << mesh.GetTransform()->GetRotationEuler().y << ", " << mesh.GetTransform()->GetRotationEuler().z << std::endl;
		std::cout << "Scale: " << mesh.GetTransform()->GetScale().x << ", " << mesh.GetTransform()->GetScale().y << ", " << mesh.GetTransform()->GetScale().z << std::endl;
		//meshWithCurrentNodeTransform = &mesh;
		_meshes.push_back(mesh);
	}

	// process children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], assimpScene, nodeTransform);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * assimpScene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

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

	/*aiMaterial* material = assimpScene->mMaterials[mesh->mMaterialIndex];

	if (material != nullptr)
	{
		
		


		std::vector<Texture*> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture*> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}*/

	return Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString filePath;
		material->GetTexture(type, i, &filePath);
		//Texture* texture = AssetLoader::LoadTexture(filePath.C_Str());
		//texture.type = typeName;
		//texture.path = str;
		//textures.push_back(texture);
	}
	return textures;
}

void Model::Render(Shader& currentShader)
{
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i].Render(currentShader);
	}
}
