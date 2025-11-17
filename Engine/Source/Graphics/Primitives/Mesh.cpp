#include "BsPrecompileHeader.h"
#include "Mesh.h"
#include "Transform.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(/*unsigned int uid,*/ const std::string& name)
    : Renderable(/*uid,*/ name)
{
    std::cout << "creating Mesh: " << Name << std::endl;
}

Mesh::Mesh(/*unsigned int uid, */std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, const std::string& name)
    : Renderable(/*uid,*/ name),_vertices(vertices), _indices(indices), Textures(textures)
{
    //std::cout << "creating Mesh: " << Name << std::endl;
	Init();
}

Mesh::~Mesh()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
   // if (EBO) glDeleteBuffers(1, &EBO);
   // std::cout << "Deleting Mesh: " << Name << std::endl;
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

void Mesh::Init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
        &_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    //vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV1));

    // unbind so that other objects can be set up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Render(Shader& currentShader)
{
    currentShader.SetUniformMat4("transform", _transform->GetObjectToWorldMatrix());

    //for now only use the first included texture as our shader only supports one shader
    if (Textures.size() > 0)
    {
        std::shared_ptr<Texture> defaultTexture = Textures[0];
        defaultTexture->Bind();
        currentShader.SetUniformInt("Texture1", 0);
    }

    //draw
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (Textures.size() > 0)
    {
        std::shared_ptr<Texture> defaultTexture = Textures[0];
        defaultTexture->UnBind();
        //currentShader.SetUniformInt("Texture1", 0);
    }
}
