#include "BsPrecompileHeader.h"
#include "FrustumMesh.h"

namespace Pixie
{
	FrustumMesh::FrustumMesh(const std::string& name )
		: Mesh(name) 
	{
		m_Vertices = {
			// back face
			Vertex{{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }}, // back top left
			Vertex{{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, { 1.0f, 1.0f }}, // back bottom left
			Vertex{{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f}, {0.5f, 0.0f, 0.5f}, { 1.0f, 1.0f }}, // back bottom right
			Vertex{{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, { 1.0f, 0.0f }}, // back top Right

			// front face
			Vertex{{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 1.0f }}, // front top left
			Vertex{{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f }}, // front bottom left
			Vertex{{ 1.0f, -1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, { 0.0f, 1.0f }}, // front bottom right
			Vertex{{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.5f}, { 0.0f, 0.0f }}, // front top right
		};

		Init();
	}

	FrustumMesh::~FrustumMesh()
	{
		if (VBO) glDeleteBuffers(1, &VBO);
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (EBO) glDeleteBuffers(1, &EBO);
	}
	void FrustumMesh::Render(Shader& currentShader)
	{}
	void FrustumMesh::Init()
	{
		//delete old if existing
		if (VBO) glDeleteBuffers(1, &VBO);
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (EBO) glDeleteBuffers(1, &EBO);

		// create new
	}
}