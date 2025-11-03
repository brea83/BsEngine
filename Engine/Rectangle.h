#pragma once
#include "Renderable.h"
#include <vector>

class Transform;

class Rectangle : public Renderable
{
public:
	Rectangle();
	Rectangle(std::vector<Vertex>& newVerticies);
	~Rectangle();

	
	void Render(Shader& currentShader) override;
protected:
	void Init() override;

};

