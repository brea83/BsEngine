#pragma once
#include "Renderable.h"
#include <vector>

class Triangle : public Renderable
{
public:
	Triangle();
	Triangle(std::vector<Vertex>& newVerticies);
	~Triangle();

	void Render(Shader& currentShader) override;

protected:
	void Init() override;
};

