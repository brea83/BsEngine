#include "BsPrecompileHeader.h"
#include "Triangle.h"
#include <glad/glad.h>
#include "Transform.h"
//#include <GLFW/glfw3.h>

static int s_NumTriangles = 0;

Triangle::Triangle()
{
	if (s_NumTriangles == 0)
	{
		Name = "Triangle";
	}
	else
	{
		Name = "Triangle_" + std::to_string(s_NumTriangles);
	}
	s_NumTriangles++;
	_vertices = {
		Vertex{{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, { 0.5f, 1.0f }},
		Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f }},
		Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f }},
	};

	_transform = new Transform();
	Init();
}

//Triangle::Triangle(std::vector<Vertex>& newVerticies)
//{
//	_vertices = newVerticies;
//	Init();
//}

void Triangle::Init(/*std::vector<Vertex>& verticies*/)
{
	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);
	// vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
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

Triangle::~Triangle()
{
	if(VBO) glDeleteBuffers(1, &VBO);
	if(VAO) glDeleteVertexArrays(1, &VAO);
}

void Triangle::Render(Shader& currentShader)
{
	// send transform to shader
	currentShader.SetUniformMat4("transform", _transform->GetObjectToWorldMatrix());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
