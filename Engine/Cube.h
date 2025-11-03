#pragma once
#include "Renderable.h"
class Cube :    public Renderable
{
public:
	Cube();
	Cube(std::vector<Vertex>& newVerticies);
	~Cube();

	// Inherited via Renderable
	void Render(Shader& currentShader) override;
protected:
	void Init() override;
};

