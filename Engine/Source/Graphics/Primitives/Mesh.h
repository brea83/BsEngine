#pragma once
#include "Renderable.h"
#include "Graphics/Texture.h"
//class Texture;

class Mesh :    public Renderable
{
public:

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Color;
		glm::vec2 UV1;
	};

	struct ObjPackedIndices
	{
		int Position;
		int Uv;
		int Normal;
	};

	//constructors and properties
	Mesh(/*unsigned int uid,*/ const std::string& name = "Default Name");
	Mesh(/*unsigned int uid,*/ std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, const std::string& name = "Default Mesh Name");
	Mesh(/*unsigned int uid,*/ std::vector<Vertex> vertices, std::vector<unsigned int> indices/*, std::vector<std::shared_ptr<Texture>> textures*/, const std::string& name = "Default Mesh Name");

	virtual ~Mesh();

	std::vector<std::shared_ptr<Texture>> Textures;

	// methods
	void Render(Shader& currentShader) override;

protected:
	//properties
	unsigned int VAO{ 0 };
	unsigned int VBO{ 0 };
	unsigned int EBO{ 0 }; //Element buffer object

	std::vector<Vertex> _vertices;
	//std::vector<glm::uvec3> _triangles;
	std::vector<unsigned int> _indices;

	//methods
	void Init() override;
};

