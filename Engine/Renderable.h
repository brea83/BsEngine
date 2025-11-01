#pragma once
#include "Shaders/Shader.h"
#include <glm.hpp>
#include <array>
#include <vector>


class Renderable
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
	};

	virtual void Render(Shader& currentShader) = 0;
protected:
	//properties
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO; //Element buffer object

	std::vector<Vertex> _verticies;
	std::vector<glm::uvec3> _triangles;
};
static const std::array<int, 2> VetexDataOffsets{ 0, sizeof(glm::vec3) };