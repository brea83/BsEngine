#include "BsPrecompileHeader.h"
#include "Model.h"
#include "AssetLoader.h"
#include "Graphics/Primitives/Transform.h"
#include "Graphics/Primitives/Mesh.h"
#include <glm/glm.hpp>
#include "Assimp/AssimpGlmHelpers.h"
#include <filesystem>
#include <fstream>
#include "GameObject.h"
#include "EngineContext.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Model::Model(GameObject* parent, const std::string& modelFilePath, const std::string& textureDirectoryPath, const std::string& textureFileName)
	: Component(parent, "Model Component"/* modelFilePath.substr(modelFilePath.find_last_of('/') + 1, modelFilePath.find_last_of("."))*/),
	_filePath(modelFilePath), 
	_texturesDirectory(textureDirectoryPath)
{

	std::string fileExtension = modelFilePath.substr(modelFilePath.find_last_of('.'));
	if (fileExtension == ".fbx")
	{
		LoadModelAssimp(modelFilePath);
	}
	else if (fileExtension == ".obj")
	{
		LoadObj(modelFilePath, textureFileName);
	}
}

Model::~Model()
{
	std::cout << "DELETING MODEL " << Name << std::endl;
	//delete[] _meshes;
}

void Model::LoadModelAssimp(const std::string & filePath)
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
	std::cout << " DIRECTORY: " << _filePath << std::endl;
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

void Model::LoadObj(const std::string& filePath, const std::string& textureFileName)
{
	
	//std::ifstream file(filePath);

	//if (!file.is_open())
	//{
	//	std::cerr << "Failed to open file: " << filePath << std::endl;
	//	return;
	//}

	//std::cout << ":::::::::::::::::::::::::::::::::::::::::::::#" << std::endl;
	//std::cout << "IMPORTING MODEL " << Name << std::endl;
	//std::cout << " DIRECTORY: " << _filePath << std::endl;
	//std::string line;

	//std::vector<Mesh::Vertex> vertices;
	//std::vector<glm::vec3> positions;
	//std::vector<glm::vec3> normals;
	//std::vector<glm::vec2> uvs;
	//std::vector<unsigned int> indices;

	//while (std::getline(file, line))
	//{
	//	std::istringstream lineStream(line);
	//	std::string firstWord;
	//	lineStream >> firstWord;

	//	if (firstWord == "v")
	//	{
	//		// do vertex stuff
	//		glm::vec3 position;
	//		lineStream >> position.x >> position.y >> position.z;
	//		positions.push_back(position);
	//	}

	//	if (firstWord == "vt")
	//	{
	//		// texture coordinates
	//		glm::vec2 uv;
	//		lineStream >> uv.x >> uv.y;
	//		uvs.push_back(uv);
	//	}

	//	if (firstWord == "vn")
	//	{
	//		// normals
	//		glm::vec3 normal;
	//		lineStream >> normal.x >> normal.y >> normal.z;
	//		normals.push_back(normal);
	//	}

	//	//if (firstWord == "vp")
	//	//{
	//	//	// parameter?
	//	//}

	//	if (firstWord == "f")
	//	{
	//		// faces data
	//		// need to double check but I think all formal exports require faces to come After all the other data
	//		//this will probably not get us quite enough spaces reserved, but it is probably closer than the empty init
	//		if (vertices.capacity() < positions.size()) vertices.reserve(positions.size());

	//		// format vertex_index/texture_index/normal_index
	//		// -1 referring to the last element of vertex list.

	//		std::vector<Mesh::ObjPackedIndices> objIndices;
	//		std::string word;
	//		while (lineStream >> word)
	//		{
	//			std::istringstream wordStream(word);
	//			std::string positionString, uvString, normalString;

	//			int relativePosIndex;
	//			int relativeUvIndex;
	//			int relativeNormalIndex;

	//			if (std::getline(wordStream, positionString, '/'))
	//			{
	//				relativePosIndex = std::atoi(positionString.c_str());
	//				relativePosIndex = std::max(0, relativePosIndex - 1);
	//			}
	//			else
	//			{
	//				relativePosIndex = 0;
	//			}

	//			if (std::getline(wordStream, uvString, '/'))
	//			{
	//				relativeUvIndex = std::atoi(uvString.c_str());
	//				relativeUvIndex = std::max(0, relativeUvIndex - 1);
	//			}
	//			else
	//			{
	//				relativeUvIndex = relativePosIndex;
	//			}

	//			if (std::getline(wordStream, normalString, '/'))
	//			{
	//				relativeNormalIndex = std::atoi(normalString.c_str());
	//				relativeNormalIndex = std::max(0, relativeNormalIndex - 1);
	//			}
	//			else
	//			{
	//				relativeNormalIndex = relativePosIndex;
	//			}

	//			int positionIndex, normalIndex, uvIndex;
	//			positionIndex = (relativePosIndex >= 0) ? relativePosIndex : positions.size() + relativePosIndex;
	//			uvIndex = (relativeUvIndex >= 0) ? relativeUvIndex : uvs.size() + relativeUvIndex;
	//			normalIndex = (relativeNormalIndex >= 0) ? relativeNormalIndex : normals.size() + relativeNormalIndex;

	//			objIndices.push_back(Mesh::ObjPackedIndices{ positionIndex, uvIndex, normalIndex });
	//		}

	//		//triangulate assuming n >3-gons are convex and coplanar
	//		for (size_t i = 1; i + 1 < objIndices.size(); i++)
	//		{
	//			const Mesh::ObjPackedIndices* point[3] = { &objIndices[0], &objIndices[i], &objIndices[i + 1] };

	//			//https://wikis.khronos.org/opengl/Calculating_a_Surface_Normal
	//			// U and V are the vectors used to calculate surface normal
	//			// U is point2 - point1 V is point3 - point1
	//			// normal is U cross V

	//			glm::vec3 U(positions[point[1]->Position] - positions[point[0]->Position]);
	//			glm::vec3 V(positions[point[2]->Position] - positions[point[0]->Position]);
	//			glm::vec3 faceNormal = glm::normalize(glm::cross(U, V));

	//			// make the vertex for the mesh

	//			for (size_t j = 0; j < 3; j++)
	//			{
	//				Mesh::Vertex vertex;

	//				vertex.Position = positions[point[j]->Position];
	//				vertex.Normal = (point[j]->Normal != 0 && normals.size() > 0) ? normals[point[j]->Normal] : faceNormal;
	//				if (uvs.size() > 0) vertex.UV1 = uvs[point[j]->Uv];

	//				// check if identical vertex exists to use its index instead
	//				auto found = std::find(vertices.begin(), vertices.end(), vertex);

	//				if (found != vertices.end())
	//				{
	//					size_t index = distance(vertices.begin(), found);
	//					indices.push_back(index);
	//				}
	//				else
	//				{
	//					indices.push_back(vertices.size());
	//					vertices.push_back(vertex);
	//				}

	//			}

	//		}

	//	}
	//}
	//file.close();

	//std::string textureFilePath = _texturesDirectory + textureFileName;
	//std::shared_ptr<Texture> texture = AssetLoader::LoadTexture(textureFilePath);
	//std::vector< std::shared_ptr<Texture>> textures;
	//std::shared_ptr<Mesh> mesh;
	//if (texture == nullptr)
	//{
	//	mesh = std::make_shared<Mesh>(vertices, indices, Name);
	//}
	//else
	//{
	//	textures.push_back(texture);
	//	mesh = std::make_shared<Mesh>(vertices, indices, textures, Name);
	//}

	std::string textureFilePath = _texturesDirectory + textureFileName;
	std::shared_ptr<Mesh> mesh = AssetLoader::LoadObj(filePath, textureFilePath);
	//std::shared_ptr<Transform> transform = mesh->GetTransform();
	//transform->ParentTransform = _parentObject->GetTransform();
	_meshes.push_back(mesh);
	
}

void Model::ProcessTransform(aiMatrix4x4 nodeMatrix, std::shared_ptr<Transform> localTransform, aiNode* parentNode)
{

	if (parentNode) 
	{ 
		localTransform->ParentTransform = _parentObject->GetTransform();
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
		ProcessTransform(node->mTransformation, _parentObject->GetTransform(), nullptr);
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

void Model::Initialize()
{

}

void Model::CleanUp()
{}

std::shared_ptr<Component> Model::Clone()
{
	return std::shared_ptr<Component>();
}

void Model::SetParentObject(GameObject* newParent)
{}

void Model::OnUpdate()
{}

void Model::Render(Shader& currentShader)
{
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i]->Render(currentShader, _parentObject->GetTransform());
	}
}
