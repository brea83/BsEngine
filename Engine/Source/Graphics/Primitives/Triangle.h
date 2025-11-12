#pragma once
#include "Mesh.h"
#include <vector>

class Triangle : public Mesh
{
public:
	Triangle();
	//Triangle(std::vector<Vertex>& newVerticies);
	~Triangle();

	void Render(Shader& currentShader) override;

protected:
	void Init() override;
};

