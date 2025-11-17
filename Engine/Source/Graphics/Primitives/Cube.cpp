#include "BsPrecompileHeader.h"
#include "Cube.h"
#include "Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static int s_NumCubes = 1;
Cube::Cube(/*unsigned int uid,*/ bool useMinimalVerticies, const std::string& name)
	: Mesh(/*uid, */(name + " " + std::to_string(s_NumCubes++)))
{
	/*if (s_NumCubes == 0)
	{
		Name = "Cube";
	}
	else
	{
		Name = "Cube_" + std::to_string(s_NumCubes);
	}
	s_NumCubes++;*/

	if (useMinimalVerticies)
	{
		_vertices = {
			// back face
			Vertex{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f }}, // top left
			Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, { 1.0f, 1.0f }}, // top right
			Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 1.0f }}, // bottom left
			Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f }}, // bottom right

			// front face
			Vertex{{-0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }}, // top left
			Vertex{{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, { 1.0f, 1.0f }}, // top right
			Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 1.0f }}, // bottom left
			Vertex{{ 0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f }}, // bottom right
		};

		//_triangles = {  //idicies of the vertexies list to use as points of a triangle
		//	// back face
		//	{0, 1, 2},   // first triangle
		//	{1, 2, 3},   // second triangle

		//	// front face
		//	{ 4, 5, 6 },   // first triangle
		//	{5, 6, 7},   // second triangle

		//	// top face
		//	{ 0, 1, 5 },   // first triangle
		//	{1, 5, 4},   // second triangle

		//	// bottom face
		//	{ 2, 3, 7 },   // first triangle
		//	{2, 7, 6},   // second triangle

		//	// left face
		//	{ 0, 4, 2 },   // first triangle
		//	{4, 2, 6},   // second triangle

		//	//right face
		//	{ 1, 5, 7 },   // first triangle
		//	{1, 3, 7},   // second triangle
		//};

		_indices = {  //idicies of the vertexies list to use as points of a triangle
			// back face
			0, 1, 2, 1, 2, 3,
			// front face
			4, 5, 6 , 5, 6, 7,   
			// top face
			0, 1, 5 , 1, 5, 4,   
			// bottom fac
			2, 3, 7 , 2, 7, 6,   
			// left face
			0, 4, 2 , 4, 2, 6,   
			//right face
			1, 5, 7 , 1, 3, 7,   
		};
	}
	else
	{
		_vertices = {
			// front face
			Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f }},
			Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f }},
			Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f }},
			Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f }},
			// back face
			Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.5f}, { 1.0f, 0.0f }},
			Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.5f}, { 1.0f, 1.0f }},
			Vertex{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.5f}, { 0.0f, 1.0f }},
			Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.5f}, { 0.0f, 0.0f }},

			//bottom face 
			Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, { 0.0f, 1.0f }},
			Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, { 1.0f, 1.0f }},
			Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, { 1.0f, 0.0f }},
			Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, { 0.0f, 0.0f }},

			//top face
			Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f }},
			Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 1.0f }},
			Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f }},
			Vertex{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, { 1.0f, 0.0f }},

			//left face
			Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, { 1.0f, 0.0f }},
			Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, { 0.0f, 0.0f }},
			Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, { 0.0f, 1.0f }},
			Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, { 1.0f, 1.0f }},

			//right face
			Vertex{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }},
			Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f }},
			Vertex{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f }},
			Vertex{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f }},
		};

		//_triangles = {
		//	// Front face
		//	{0, 2, 1}, {2, 0, 3},
		//	// Back face
		//	{4, 6, 5}, {6, 4, 7},
		//	// Bottom face
		//	{8, 10, 9}, {10, 8, 11},
		//	// Top face
		//	{12, 14, 13}, {14, 12, 15},
		//	// Left face
		//	{16, 18, 17 }, { 18, 16, 19 },
		//	// Right face
		//	{20, 22, 21}, {22, 20, 23}
		//};

		_indices = {
			// Front face
			0, 2, 1, 2, 0, 3,
			// Back face
			4, 6, 5, 6, 4, 7,
			// Bottom face
			8, 10, 9, 10, 8, 11,
			// Top face
			12, 14, 13, 14, 12, 15,
			// Left face
			16, 18, 17 , 18, 16, 19 ,
			// Right face
			20, 22, 21, 22, 20, 23
		};
	}

	_transform = std::make_shared<Transform>();

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
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	// set up element buffer object, which gets saved on the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Position));
	glEnableVertexAttribArray(0);
	//vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Color));
	glEnableVertexAttribArray(1);
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//vertex uv
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV1));
	glEnableVertexAttribArray(3);

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
	//_transform->Rotate( 0.02f, glm::vec3(0.0f, 0.0f, 1.0f));
	// send transform to shader
	currentShader.SetUniformMat4("transform", _transform->GetObjectToWorldMatrix());

	//draw
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
