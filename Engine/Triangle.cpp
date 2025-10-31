#include "Triangle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Triangle::Triangle()
{
	std::vector<float> verticiesOld = {
		 0.0f,  0.5f, 0.0,   /* top */   1.0, 0.0, 0.0,
		-0.5f, -0.5f, 0.0f, /* left */  0.0, 1.0, 0.0,
		 0.5f, -0.5f, 0.0f, /* right*/  0.0, 0.0, 1.0,
	};

	std::vector<Vertex> verticies = {
		Vertex{{0.0f,  0.5f, 0.0}, {1.0, 0.0, 0.0}},
		Vertex{{-0.5f, -0.5f, 0.0f}, {0.0, 1.0, 0.0}},
		Vertex{{0.5f, -0.5f, 0.0f}, {0.0, 0.0, 1.0}},
	};

	Init(verticies);
}

Triangle::Triangle(std::vector<Vertex>& newVerticies)
{
	Init(newVerticies);
}

void Triangle::Init(std::vector<Vertex>& verticies)
{
	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VetexDataOffsets[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VetexDataOffsets[1]);
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
	//shader.SetUniformMat4("u_Model", m_Transform);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
