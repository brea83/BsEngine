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
        std::cout << "creating Mesh: " << Name << std::endl;
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

        std::cout << "initialized mesh with " << m_Vertices.size() << " vertices" << std::endl;
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
            std::cout << "Warning serialization version doesn't match." << std::endl;
            std::cout << "Pixie Engine Expects: " << s_Version << std::endl;
            std::cout << "Found: " << version << std::endl;
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

    void Mesh::CalculateTangents()
    {
        for (int i = 0; i < m_Indices.size(); i += 3)
        {
            //the three verts of our tri
            unsigned int index0 = m_Indices[i];
            unsigned int index1 = m_Indices[i + 1];
            unsigned int index2 = m_Indices[i + 2];

            Vertex& v0 = m_Vertices[index0];
            Vertex& v1 = m_Vertices[index1];
            Vertex& v2 = m_Vertices[index2];

            //edges of the tri
            glm::vec3 edge1 = v1.Position - v0.Position;
            glm::vec3 edge2 = v2.Position - v0.Position;

            // uv deltas
            glm::vec2 deltaUV1 = v1.UV1 - v0.UV1;
            glm::vec2 deltaUV2 = v2.UV1 - v0.UV1;

            //compute tangent and bitangent

            float denominator = (deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x);
            denominator = 1.0f / denominator;

            v0.Tangent = ((edge1 * deltaUV2.y) - (edge2 * deltaUV1.y)) * denominator;
            v0.Tangent = glm::normalize(v0.Tangent - (v0.Normal * glm::dot(v0.Normal, v0.Tangent)));
            v1.Tangent = glm::normalize(v0.Tangent - (v1.Normal * glm::dot(v1.Normal, v0.Tangent)));
            v2.Tangent = glm::normalize(v0.Tangent - (v2.Normal * glm::dot(v2.Normal, v0.Tangent)));

            v0.BiTangent = glm::cross(v0.Normal, v0.Tangent);//((edge2 * deltaUV1.x) - (edge1 * deltaUV2.x)) * denominator;


            v1.BiTangent = v0.BiTangent;
            v2.BiTangent = v0.BiTangent;
        }
    }
}