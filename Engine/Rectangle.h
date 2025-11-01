#pragma once
#include "Renderable.h"
#include <vector>

class Rectangle : public Renderable
{
public:
	Rectangle();
	Rectangle(std::vector<Vertex>& newVerticies);
	~Rectangle();

	void Render(Shader& currentShader) override;
private:
	

	// methods
	void Init();
};

