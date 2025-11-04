#include "Triangle.h"
#include <glad/glad.h>
#include "Transform.h"
//#include <GLFW/glfw3.h>

Triangle::Triangle()
{
	_verticies = {
		Vertex{{ 0.0f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	};

	_transform = new Transform();
	Init();
}

Triangle::Triangle(std::vector<Vertex>& newVerticies)
{
	_verticies = newVerticies;
	Init();
}

void Triangle::Init(/*std::vector<Vertex>& verticies*/)
{
	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  _verticies.size() * sizeof(Vertex), &_verticies[0], GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

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
