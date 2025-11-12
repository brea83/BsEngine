#pragma once
#include "Renderable.h"

class Texture;

class Mesh :    public Renderable
{
public:

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec2 UV1;
		glm::vec3 Normal;
	};

	//constructors and properties
	Mesh(const std::string& name = "Default Name");
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);

	std::vector<Texture*> Textures;

	// methods
	void Render(Shader& currentShader) override;

protected:
	//properties
	unsigned int VAO{ 0 };
	unsigned int VBO{ 0 };
	unsigned int EBO{ 0 }; //Element buffer object

	std::vector<Vertex> _vertices;
	std::vector<glm::uvec3> _triangles;
	std::vector<unsigned int> _indices;

	//methods
	void Init() override;
};

