#include "Triangle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Triangle::Triangle()
{
	float verticies[] = {
		 0.0f,  0.5f, 0.0,   // top  
		-0.5f, -0.5f, 0.0f, // left
		 0.5f, -0.5f, 0.0f, // right
	};

	// generate vert array and vert buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind vert array first, then bind the buffer and tell it how ot traverse the array.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

Triangle::~Triangle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Triangle::Render()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
