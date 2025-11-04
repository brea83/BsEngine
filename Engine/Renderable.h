#pragma once
#include "Shaders/Shader.h"
#include <glm/glm.hpp>
#include <array>
#include <vector>

class Transform;

class Renderable
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec2 UV1;
		//glm::vec3 Normals;
	};


	virtual Transform* GetTransform() { return _transform; }
	virtual void Render(Shader& currentShader) = 0;
protected:
	//properties

	Transform* _transform;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO; //Element buffer object

	std::vector<Vertex> _verticies;
	std::vector<glm::uvec3> _triangles;

	// methods

	// methods
	virtual void Init() = 0;
};
//static const std::array<int, 2> VetexDataOffsets{ 0, sizeof(glm::vec3) };