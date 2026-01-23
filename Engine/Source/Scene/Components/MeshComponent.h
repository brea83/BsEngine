#pragma once
#include "Core.h"
#include "BsPrecompileHeader.h"
//#include "Scene/Components/Component.h"
#include "Resources/FileStream.h"
#include "MaterialInstance.h"
#include "Graphics/Primitives/Mesh.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Pixie
{
	class GameObject;

	class MeshComponent
	{
	public:
		using StdPath = std::filesystem::path;
		MeshComponent();
		MeshComponent(PrimitiveMeshType primitiveMesh);
		MeshComponent(const std::string& modelFilePath, const std::string& textureFilePath = "");
		~MeshComponent();

		//static constexpr SerializableComponentID ID{ SerializableComponentID::MeshComponent };
		bool Reload();

		void SetMesh(std::shared_ptr<Mesh>& mesh) { m_Mesh = mesh; }

		const std::string& Name() const { return m_Name; }
		void SetName(const std::string& name)  { m_Name = name; }

		void SetFilePath(const std::string& path) { m_FilePath = path; }
		std::string GetFilePath() { return m_FilePath; }

		MaterialInstance& GetMaterialInstance() { return m_MaterialInstance; }

		bool HasTexture() { return (m_MaterialInstance.BaseMap != nullptr ) && (m_MaterialInstance.BaseMap->TextureObject != 0); }

		void OnUpdate() ;
		
		void Render(Shader& currentShader);
		void RenderWithoutMaterial(Shader& currentShader);

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
				component.m_MaterialInstance.Reload();
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

		friend class DetailsViewPanel;
	};

	struct CircleRendererComponent
	{
		CircleRendererComponent();
		CircleRendererComponent(const glm::vec4& color);

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Radius{ 0.5f };
		float LineWidth{ 0.01f }; // range 0 to 1. 1 = filled circle
		float Fade{ 0.005f }; // softens and blurs edges of circle
		static void Serialize(StreamWriter* stream, const CircleRendererComponent& component);
		static bool Deserialize(StreamReader* stream, CircleRendererComponent& component);
		std::shared_ptr<Mesh> MeshResource { nullptr };
	};
}


