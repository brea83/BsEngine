#pragma once
#include "Mesh.h"
#include <vector>

class Transform;

class Quad : public Mesh
{
public:
	Quad(/*unsigned int uid, */const std::string& name = "Rectangle");
	~Quad();

	
	void Render(Shader& currentShader) override;
protected:
	void Init() override;

};

