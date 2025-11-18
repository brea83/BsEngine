#include "BsPrecompileHeader.h"
#include "Model.h"
#include "AssetLoader.h"
#include "Graphics/Primitives/Transform.h"
#include "Graphics/Primitives/Mesh.h"
#include <glm/glm.hpp>
#include "Assimp/AssimpGlmHelpers.h"
#include <filesystem>
#include <fstream>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Model::Model(/*unsigned int uid, */const std::string& modelFilePath, const std::string& textureDirectoryPath)
	: Renderable(/*uid, */modelFilePath.substr(modelFilePath.find_last_of('/') + 1, modelFilePath.find("."))), _directory(modelFilePath.substr(0, modelFilePath.find_last_of('/'))), _texturesDirectory(textureDirectoryPath)
{
	//_directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));
	//Name = modelFilePath.substr(modelFilePath.find_last_of('/') + 1, modelFilePath.find("."));
	//_uid = uid;
	LoadModel(modelFilePath);
}

Model::~Model()
{
	//delete[] _meshes;
}

void Model::Init()
{}

void Model::LoadModel(const std::string & filePath)
{
	//Parse filepath for .fbx or .obj and send it to different loaders.
	std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
	if (fileExtension == ".fbx")
	{
		// TODO: turn this into a function
		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace);

		if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
		std::cout << "IMPORTING MODEL " << Name << std::endl;
		std::cout << " DIRECTORY: " << _directory << std::endl;
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
		//std::cout << "Reserving " << assimpScene->mNumMeshes << ", in _meshes" << std::endl;
		//std::cout << "====================================" << std::endl;
		_meshes.reserve(assimpScene->mNumMeshes);
		ProcessNode(assimpScene->mRootNode, assimpScene, aiMatrix4x4());
		return;
	}
	
	if( fileExtension == ".obj")
	{
		//std::shared_ptr< TextResource> fileText = AssetLoader::LoadTextFile(filePath);


		// no stored text asset from that path so make one
		std::ifstream file(filePath);

		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filePath << std::endl;
			return;
		}
		std::string line;

		std::vector<Mesh::Vertex> vertices;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indices;

		while(std::getline(file, line))
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
				// format vertex_index/texture_index/normal_index
				// -1 referring to the last element of vertex list.
				
				std::vector<Mesh::ObjPackedIndices> objIndices;
				std::string word;
				while (lineStream >> word)
				{
					std::istringstream wordStream(word);
					std::string positionString, uvString, normalString;
					std::getline(wordStream, positionString, '/');
					std::getline(wordStream, uvString, '/');
					std::getline(wordStream, normalString, '/');

					int relativePosIndex = std::atoi(positionString.c_str());
					int relativeUvIndex = std::atoi(uvString.c_str());
					int relativeNormalIndes = std::atoi(normalString.c_str());

					unsigned int positionIndex, normalIndex, uvIndex;
					positionIndex = (relativePosIndex >= 0) ? relativePosIndex : positions.size() + relativePosIndex;
					uvIndex = (relativeUvIndex >= 0) ? relativeUvIndex : uvs.size() + relativeUvIndex;
					normalIndex = (relativeNormalIndes >= 0) ? relativeNormalIndes : normals.size() + relativeNormalIndes;
					
					objIndices.push_back(Mesh::ObjPackedIndices{ positionIndex, uvIndex, normalIndex });
				}


				

				/*indices.push_back(positionIndex);
				lineStream >> word;
				indices.push_back(normalIndex);
				lineStream >> word;
				indices.push_back(uvIndex);*/
			}
		}
		file.close();

		vertices.reserve(positions.size());
		for (int i = 0; i < positions.size(); i++)
		{
			Mesh::Vertex vertex;
			vertex.Position = positions[i];

			if (i < normals.size()) vertex.Normal = normals[i];

			if (i < uvs.size()) vertex.UV1 = uvs[i];
			
			vertices.push_back(vertex);
		}
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, indices, Name);
		std::shared_ptr<Transform> transform = mesh->GetTransform();
		transform->ParentTransform = _transform;
		_meshes.push_back(mesh);
	}



}

void Model::ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<Transform> localTransform, aiNode* parentNode)
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
		ProcessTransform(nodeTransform, mesh->GetTransform(), node->mParent);
		//std::cout << "Position: " << mesh.GetTransform()->GetPosition().x << ", " << mesh.GetTransform()->GetPosition().y << ", " << mesh.GetTransform()->GetPosition().z << std::endl;
		//std::cout << "Rotation: " << mesh.GetTransform()->GetRotationEuler().x << ", " << mesh.GetTransform()->GetRotationEuler().y << ", " << mesh.GetTransform()->GetRotationEuler().z << std::endl;
		//std::cout << "Scale: " << mesh.GetTransform()->GetScale().x << ", " << mesh.GetTransform()->GetScale().y << ", " << mesh.GetTransform()->GetScale().z << std::endl;
		//meshWithCurrentNodeTransform = &mesh;
		_meshes.push_back(mesh);
	}

	// process children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], assimpScene, nodeTransform);
	}
}

std::shared_ptr<Mesh>  Model::processMesh(aiMesh * mesh, const aiScene * assimpScene)
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

	return std::make_shared<Mesh>(/*_uid, */vertices, indices, textures, mesh->mName.C_Str());
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType bsTextureType)
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

void Model::Render(Shader& currentShader)
{
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i]->Render(currentShader);
	}
}
