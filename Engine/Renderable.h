#pragma once
#include "Shaders/Shader.h"
#include <glm.hpp>
#include <array>


class Renderable
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
	};

	virtual void Render(Shader& currentShader) = 0;
};
static const std::array<int, 2> VetexDataOffsets{ 0, sizeof(glm::vec3) };