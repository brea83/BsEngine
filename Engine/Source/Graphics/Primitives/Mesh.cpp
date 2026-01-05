#include "BsPrecompileHeader.h"
#include "Mesh.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Pixie
{
    Mesh::Mesh(const std::string& name)
        : Renderable(name)
    {
        Logger::Log(LOG_TRACE, "creating Mesh: {}", name);
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string& name)
        : Renderable(name), m_Vertices(vertices), m_Indices(indices)
    {
        Init();
    }

    Mesh::~Mesh()
    {
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
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
            &m_Indices[0], GL_STATIC_DRAW);

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

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));

        // unbind so that other objects can be set up
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Logger::Log(LOG_TRACE, "initialized mesh with {} vertices", m_Vertices.size());
    }

    void Mesh::Render(Shader& currentShader)
    {

        //draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    void Mesh::Serialize(StreamWriter* stream, const Mesh& mesh)
    {
        stream->WriteRaw<int>(s_Version);
        stream->WriteArray<Vertex>(mesh.m_Vertices);
        stream->WriteArray<unsigned int>(mesh.m_Indices);
    }

    bool Mesh::Deserialize(StreamReader* stream, Mesh& mesh)
    {
        int version{ -1 };
        stream->ReadRaw<int>(version);
        if (version != s_Version)
        {
            Logger::Log(LOG_WARNING, "Warning serialization version doesn't match.");
            Logger::Log(LOG_WARNING, "Pixie Engine Expects: {}", s_Version);
            Logger::Log(LOG_WARNING, "Found: {}", version);
        }
        
        // manually reading out the size of the stored arrays to error check before getting the array data itself
        int vertexCount{ -1 };
        stream->ReadRaw<int>(vertexCount);
        if (vertexCount > 100000000) throw std::runtime_error("Error reading serialized mesh, too many vertices");

        stream->ReadArray<Vertex>(mesh.m_Vertices, vertexCount);

        int indexCount{ -1 };
        stream->ReadRaw<int>(indexCount);
        if (indexCount > 300000000) throw std::runtime_error("Error reading serialized mesh, too many indices");
        
        stream->ReadArray<unsigned int>(mesh.m_Indices, indexCount);
        return true;
    }

    void Mesh::Vertex::OrthagonalizeTangent()
    {
        Tangent = glm::normalize(Tangent - (Normal * glm::dot(Normal, Tangent)));
    }
    void Mesh::Vertex::CalculateBitangent()
    {
        BiTangent = glm::cross(Normal, Tangent); 
    }
}