#include "Rectangle.h"
#include <glad/glad.h>

Rectangle::Rectangle()
{
	_verticies = {
		Vertex{{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // top left
		Vertex{{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}}, // top right
		Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // bottom left
		Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // bottom right
	};

	_triangles = {  //idicies of the vertexies list to use as points of a triangle
		{0, 1, 2},   // first triangle
		{1, 2, 3}   // second triangle
	};

	Init();
}

Rectangle::Rectangle(std::vector<Vertex>& newVerticies)
{
	_verticies = newVerticies;
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
	glBufferData(GL_ARRAY_BUFFER, _verticies.size() * sizeof(Vertex), &_verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangles.size() * sizeof(glm::uvec3), &_triangles[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VetexDataOffsets[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VetexDataOffsets[1]);
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
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}