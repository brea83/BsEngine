#include "Cube.h"
#include "Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Cube::Cube(bool useMinimalVerticies)
{
	if (useMinimalVerticies)
	{
		_verticies = {
			// back face
			Vertex{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // top left
			Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}}, // top right
			Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // bottom left
			Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // bottom right

			// front face
			Vertex{{-0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}, // top left
			Vertex{{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}, // top right
			Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, // bottom left
			Vertex{{ 0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}, // bottom right
		};

		_triangles = {  //idicies of the vertexies list to use as points of a triangle
			// back face
			{0, 1, 2},   // first triangle
			{1, 2, 3},   // second triangle

			// front face
			{ 4, 5, 6 },   // first triangle
			{5, 6, 7},   // second triangle

			// top face
			{ 0, 1, 5 },   // first triangle
			{1, 5, 4},   // second triangle

			// bottom face
			{ 2, 3, 7 },   // first triangle
			{2, 7, 6},   // second triangle

			// left face
			{ 0, 4, 2 },   // first triangle
			{4, 2, 6},   // second triangle

			//right face
			{ 1, 5, 7 },   // first triangle
			{1, 3, 7},   // second triangle
		};
	}
	else
	{
		_verticies = {
			// front face
			Renderable::Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
			Renderable::Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
			Renderable::Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
			Renderable::Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
			// back face
			Renderable::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.5f}},
			Renderable::Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.5f}},
			Renderable::Vertex{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.5f}},
			Renderable::Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.5f}},

			//bottom face 
			Renderable::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.0f}},
			Renderable::Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.0f}},
			Renderable::Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.5f, 0.0f}},
			Renderable::Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.5f, 0.0f}},

			//top face
			Renderable::Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			Renderable::Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
			Renderable::Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
			Renderable::Vertex{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},

			//left face
			Renderable::Vertex{{-0.5f, -0.5f, -0.5f}, {0.5f, 0.0f, 0.0f}},
			Renderable::Vertex{{-0.5f, -0.5f,  0.5f}, {0.5f, 0.0f, 0.0f}},
			Renderable::Vertex{{-0.5f,  0.5f,  0.5f}, {0.5f, 0.0f, 0.0f}},
			Renderable::Vertex{{-0.5f,  0.5f, -0.5f}, {0.5f, 0.0f, 0.0f}},

			//right face
			Renderable::Vertex{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			Renderable::Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			Renderable::Vertex{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
			Renderable::Vertex{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
		};

		_triangles = {
			// Front face
			{0, 2, 1}, {2, 0, 3},
			// Back face
			{4, 6, 5}, {6, 4, 7},
			// Bottom face
			{8, 10, 9}, {10, 8, 11},
			// Top face
			{12, 14, 13}, {14, 12, 15},
			// Left face
			{16, 18, 17 }, { 18, 16, 19 },
			// Right face
			{20, 22, 21}, {22, 20, 23}
		};
	}

	_transform = new Transform();

	Init();
}

void Cube::Init()
{
	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _verticies.size() * sizeof(Vertex), &_verticies[0], GL_STATIC_DRAW);

	// set up element buffer object, which gets saved on the VAO
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

Cube::~Cube()
{
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Cube::Render(Shader& currentShader)
{
	// rotate transform on tick TEMP DO NOT LEAVE THIS IN
	_transform->Rotate( 0.05f, glm::vec3(1.0f, 1.0f, 0.0f));
	// send transform to shader
	currentShader.SetUniformMat4("transform", _transform->GetObjectToWorldMatrix());

	//draw
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, _triangles.size() * sizeof(glm::uvec3), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
