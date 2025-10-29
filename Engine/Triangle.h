#pragma once
#include "Renderable.h"

class Triangle : public Renderable
{
public:
	Triangle();
	~Triangle();

	void Render() override;
private:
	unsigned int VAO;
	unsigned int VBO;

};

