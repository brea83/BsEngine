#include "BsPrecompileHeader.h"
#include "CircleMesh.h"

namespace Pixie
{
	const std::vector<CircleVertex> CircleMesh::m_CircleVertices =
	{
		CircleVertex{{-1.0f, -1.0f, 0.0f}},//, {0.0f, 0.0f, 1.0f} } // bottom left
		CircleVertex{{ 1.0f, -1.0f, 0.0f}},//, {0.0f, 0.0f, 1.0f} }, // bottom right
		CircleVertex{{ 1.0f,  1.0f, 0.0f}},//, {0.0f, 0.0f, 1.0f} }, // top right
		CircleVertex{{-1.0f,  1.0f, 0.0f}},//, {0.0f, 0.0f, 1.0f} }, // top left
	};									
	const std::vector<uint32_t> CircleMesh::m_CircleIndices = { 0, 1, 2,  2, 3, 0 };


	CircleMesh::CircleMesh(const std::string& name)
		: Mesh(name) 
	{
		Init();
	}

	CircleMesh::~CircleMesh()
	{
		if (VBO) glDeleteBuffers(1, &VBO);
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (EBO) glDeleteBuffers(1, &EBO);
	}
	
	void CircleMesh::Init()
	{
		// generate vert array and vert buffer
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);


		//bind vert array first, then bind the buffer and tell it how ot traverse the array.
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_CircleVertices.size() * sizeof(CircleVertex), &m_CircleVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_CircleIndices.size() * sizeof(uint32_t), &m_CircleIndices[0], GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)offsetof(CircleVertex, LocalPosition));
		glEnableVertexAttribArray(0);

		// unbind so that other objects can be set up
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	void CircleMesh::Render(Shader & currentShader)
	{
		//draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_CircleIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}