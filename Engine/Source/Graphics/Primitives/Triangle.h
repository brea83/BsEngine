#pragma once
#include "Mesh.h"
#include <vector>

class Triangle : public Mesh
{
public:
	Triangle(/*unsigned int uid*/const std::string& name = "Triangle");
	//Triangle(std::vector<Vertex>& newVerticies);
	~Triangle();

	void Render(Shader& currentShader) override;

protected:
	void Init() override;
};

