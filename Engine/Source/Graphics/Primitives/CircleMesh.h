#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"
namespace Pixie
{
	struct CircleVertex
	{
		//glm::vec3 WorldPosition; the example was using this for batching, which I don't have set up yet
		glm::vec3 LocalPosition;
	};

	struct CircleUniforms
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float LineWidth{ 0.01f };
		float Fade{ 0.005f };
	};

	class CircleMesh : public Mesh
	{
	public:
		CircleMesh(const std::string& name = "Circle");
		~CircleMesh();

		virtual void Render(Shader& currentShader) override;
	protected:
		virtual void Init() override;
		static const std::vector<CircleVertex> m_CircleVertices;
		static const std::vector<uint32_t> m_CircleIndices;
	};
}