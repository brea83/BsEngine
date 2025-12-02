#pragma once
#include "Renderable.h"
#include "Graphics/Texture.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

enum class PrimitiveMeshType : int
{
	Triangle,
	Quad,
	Cube
};

class Mesh :    public Renderable
{
public:

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Color;
		glm::vec2 UV1;

		bool operator==(Vertex otherVertex)
		{
			if (otherVertex.Position == this->Position 
				&& otherVertex.Normal == this->Normal 
				&& otherVertex.UV1 == this->UV1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	};

	struct ObjPackedIndices
	{
		int Position;
		int Uv;
		int Normal;
	};

	//constructors and properties
	Mesh( const std::string& name = "Default Name");
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string& name = "Default Mesh Name");

	virtual ~Mesh();

	std::vector<Vertex>& GetVertices() { return m_Vertices; }
	std::vector<unsigned int>& GetIndices() { return m_Indices; }
	// methods
	void Render(Shader& currentShader) override;
	
protected:
	//properties
	unsigned int VAO{ 0 };
	unsigned int VBO{ 0 };
	unsigned int EBO{ 0 }; //Element buffer object

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	//methods
	void Init() override;

};

template <>
struct std::hash<Mesh::Vertex>
{
	std::size_t operator()(const Mesh::Vertex& k) const
	{
	/*	using std::size_t;
		using std::hash;
		using std::string;*/

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return ((std::hash<glm::vec3>()(k.Position)
			^ (std::hash<glm::vec3>()(k.Normal) << 1)) >> 1)
			^ (std::hash<glm::vec2>()(k.UV1) << 1);
	}
};