#pragma once
#include "Mesh.h"
namespace Pixie
{
	class FrustumMesh : public Mesh
	{
		FrustumMesh(const std::string& name = "Frustum Mesh");
		~FrustumMesh();

		//inherited from renderable 
		void Render(Shader& currentShader) override;
	protected:
		void Init() override;
	private:
		friend class WireframePass;
	};
}