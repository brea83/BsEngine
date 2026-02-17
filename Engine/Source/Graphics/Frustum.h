#pragma once
#include <glm/glm.hpp>
#include <memory>
namespace Pixie
{
	class Shader;
	class GameObject;
	class Frustum
	{
	public:
		Frustum() = default;
		//Frustum(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
		//Frustum(const Frustum&) = default;
		bool IsInitialized() { return m_Initialized; }
		void Initialize(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
		{
			Recalculate(projectionMatrix, viewMatrix);
			m_Initialized = true;
		}
		void Recalculate(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
		void Recalculate(GameObject& cameraObject);
		const std::vector<glm::vec4>& GetCorners() const { return m_Corners; }
		glm::vec3 GetCenter() const { return m_Center; }
		const glm::mat4& GetMatrix() const { return m_Matrix; }
		glm::mat4 GetMatrixCopy() const { return m_Matrix; }

		static std::vector<glm::vec4> CalcFrustumCornersWS(const glm::mat4& projection, const glm::mat4& view);
		static glm::vec3 CalcFrustumCenter(const std::vector<glm::vec4>& frustumCorners);
		static glm::mat4 CalcFrustumMatrix(const glm::mat4& projection, const glm::mat4& view);

		//void Render(std::shared_ptr<Shader> shader);
	private:

		//invert this to transform a cube (-1 to 1) into the shape of the frustum
		glm::mat4 m_Matrix{};
		std::vector<glm::vec4> m_Corners{};
		glm::vec3 m_Center{ 0.0f };

		bool m_Initialized{ false };
		void SetCorners();
	};
}