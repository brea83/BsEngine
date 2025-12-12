#pragma once
#include "Mesh.h"
#include <vector>

namespace Pixie
{

	class Quad : public Mesh
	{
	public:
		Quad(/*unsigned int uid, */const std::string& name = "Rectangle");
		~Quad();


		void Render(Shader& currentShader) override;
	protected:
		void Init() override;

	};

}