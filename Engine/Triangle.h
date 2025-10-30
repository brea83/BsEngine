#pragma once
#include "Renderable.h"
#include <vector>

class Triangle : public Renderable
{
public:
	Triangle();
	Triangle(std::vector<float>& newVerticies);
	~Triangle();

	void Render(Shader& currentShader) override;
private:
	//properties
	unsigned int VAO;
	unsigned int VBO;

	// methods
	void Init(std::vector<float>& verticies);
};

