#include "BsPrecompileHeader.h"
#include "QuadMesh.h"
#include "Transform.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Quad::Quad(const std::string& name)
	: Mesh(name)
{
	m_Vertices = {
		Vertex{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f }}, // top left
		Vertex{{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 1.0f }}, // top right
		Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f }}, // bottom left
		Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // bottom right
	};

	m_Indices = {
		0, 1, 2,
		1, 2, 3
	};


	Init();
}

void Quad::Init()
{

	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

	// unbind so that other objects can be set up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Quad::~Quad()
{
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Quad::Render(Shader& currentShader)
{
	//draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}