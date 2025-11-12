#pragma once
#include "Mesh.h"
#include <vector>

class Transform;

class Rectangle : public Mesh
{
public:
	Rectangle();
	Rectangle(std::vector<Vertex>& newVerticies);
	~Rectangle();

	
	void Render(Shader& currentShader) override;
protected:
	void Init() override;

};

