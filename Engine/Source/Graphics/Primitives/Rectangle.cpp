#include "BsPrecompileHeader.h"
#include "Rectangle.h"
#include "Transform.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Rectangle::Rectangle()
{
	_vertices = {
		Vertex{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f }}, // top left
		Vertex{{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 1.0f }}, // top right
		Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f }}, // bottom left
		Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // bottom right
	};

	//_triangles = {  //idicies of the vertexies list to use as points of a triangle
	//	{0, 1, 2},   // first triangle
	//	{1, 2, 3}   // second triangle
	//};
	_indices = {
		0, 1, 2,
		1, 2, 3
	};

	_transform = new Transform();

	Init();
}

Rectangle::Rectangle(std::vector<Vertex>& newVerticies)
{
	_vertices = newVerticies;
	_transform = new Transform();
	Init();
}

void Rectangle::Init()
{

	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

	// unbind so that other objects can be set up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Rectangle::~Rectangle()
{
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Rectangle::Render(Shader& currentShader)
{
	// send transform to shader
	currentShader.SetUniformMat4("transform", _transform->GetObjectToWorldMatrix());

	//draw
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}