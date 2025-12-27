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
 : m_Name("Mesh Component"), m_FilePath("")
	{ }

	MeshComponent::MeshComponent(PrimitiveMeshType primitiveMesh)
	: m_Name("Primitive Mesh Component")
	{
		std::shared_ptr<Mesh> mesh = AssetLoader::LoadPrimitive(primitiveMesh);
		if (mesh != nullptr)
		{
			m_Mesh = mesh;

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
		StdPath path(m_FilePath);
		StdPath fileExtension = path.extension();
			GameObject rootObject;
			EngineContext* engine = EngineContext::GetEngine();
			Scene* scene = engine->GetScene();
			entt::registry& registry = scene->GetRegistry();


			auto view = registry.view<MeshComponent>();

			for (auto entity : view)
			{
				MeshComponent& component = view.get<MeshComponent>(entity);
				if (component == *this)
				{
					rootObject = GameObject(entity, scene);
					break;
				}
			}

			if (rootObject.GetScene())
			{
				return AssetLoader::LoadMesh(rootObject, *this, path);
			}
			return false;
	}

	MeshComponent::~MeshComponent()
	{
		std::cout << "DELETING " << m_Name << std::endl;
	}

	//bool MeshComponent::LoadObj(const StdPath& filePath, const std::string& textureFileName)
	//{
	//	std::shared_ptr<Mesh> mesh = AssetLoader::LoadMesh(filePath);
	//	if (mesh == nullptr)
	//	{
	//		return false;
	//	}
	//	m_Mesh = mesh;

	//	if (textureFileName != "")
	//	{
	//		m_MaterialInstance.BaseMapPath = textureFileName;
	//		m_MaterialInstance.BaseMap = AssetLoader::LoadTexture(textureFileName);
	//		if (m_MaterialInstance.BaseMap == nullptr) return false;
	//	}

	//	return true;
	//}

	void MeshComponent::OnUpdate()
	{}

	void MeshComponent::Render(Shader& currentShader)
	{
		if (!m_Mesh) return;

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

			currentShader.SetUniformBool("material.bMapIsRoughness", m_MaterialInstance.BMapIsRoughness);
			
		}
		else
		{
			currentShader.SetUniformBool("material.bUseMetalicMap", false);
		}

		currentShader.SetUniformFloat("material.specularPower", m_MaterialInstance.SpecularPower);
		currentShader.SetUniformFloat("material.smoothness", m_MaterialInstance.Smoothness);

		m_Mesh->Render(currentShader);

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
