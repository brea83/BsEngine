#include "BsPrecompileHeader.h"
#include "Triangle.h"
#include <glad/glad.h>
//#include <GLFW/glfw3.h>

namespace Pixie
{
	static int s_NumTriangles = 1;

	Triangle::Triangle(/*unsigned int uid*/const std::string& name)
		: Mesh(/*uid*/name + " " + std::to_string(s_NumTriangles++))
	{
		m_Vertices = {
			Vertex{{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, { 0.5f, 1.0f }},
			Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f }},
			Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f }},
		};

		Init();
	}

	void Triangle::Init()
	{
		// generate vert array and vert buffer
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		//bind vert array first, then bind the buffer and tell it how ot traverse the array.
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

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
		if (VBO) glDeleteBuffers(1, &VBO);
		if (VAO) glDeleteVertexArrays(1, &VAO);
	}

	void Triangle::Render(Shader& currentShader)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}