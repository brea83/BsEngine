#pragma once
#include "Mesh.h"
class Cube :    public Mesh
{
public:
	//Cube();
	Cube(bool useMinimalVerticies = true);
	//Cube(std::vector<Vertex>& newVerticies, std::vector<glm::uvec3>& newTriangleIndices);
	~Cube();

	// Inherited via Renderable
	void Render(Shader& currentShader) override;
protected:
	void Init() override;
};

