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

	Transform* GetTransform() { return _transform; }
	void Render(Shader& currentShader) override;
private:

	Transform* _transform;
	
	// methods
	void Init();
};

