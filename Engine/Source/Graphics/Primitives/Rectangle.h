#pragma once
#include "Mesh.h"
#include <vector>

class Transform;

class Rectangle : public Mesh
{
public:
	Rectangle(/*unsigned int uid, */const std::string& name = "Rectangle");
	~Rectangle();

	
	void Render(Shader& currentShader) override;
protected:
	void Init() override;

};

