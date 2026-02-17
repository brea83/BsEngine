#include "BsPrecompileHeader.h"
#include "Frustum.h"
#include "Shaders/Shader.h"
#include "Scene/GameObject.h"

namespace Pixie
{

	//Frustum::Frustum(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	//{
	//	m_Corners = CalcFrustumCornersWS(projectionMatrix, viewMatrix);
	//}

	void Frustum::Recalculate(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		//m_Corners = CalcFrustumCornersWS(projectionMatrix, viewMatrix);
		m_Matrix = (projectionMatrix * viewMatrix);
		SetCorners();
		m_Center = CalcFrustumCenter(m_Corners);

		if (!m_Initialized)
			m_Initialized = true;
	}

	void Frustum::Recalculate(GameObject& cameraObject)
	{
		glm::mat4 projection = cameraObject.GetComponent<CameraComponent>().Cam.CalcProjectionMatrix();
		glm::mat4 view = glm::inverse(cameraObject.GetTransform().GetModelMatrix());
		Recalculate(projection, view);
	}

	std::vector<glm::vec4> Frustum::CalcFrustumCornersWS(const glm::mat4& projection, const glm::mat4& view)
    {
		const auto inverse = glm::inverse(projection * view);

		std::vector<glm::vec4> frustumCorners;
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					glm::vec4 point
					{
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * z - 1.0f,
						1.0f
					};

					point = inverse * point;

					frustumCorners.push_back(point / point.w);
				}
			}
		}

		return frustumCorners;
    }
    
	glm::vec3 Frustum::CalcFrustumCenter(const std::vector<glm::vec4>& frustumCorners)
    {
		glm::vec3 center{ 0.0f };
		for (const auto& point : frustumCorners)
		{
			center += glm::vec3(point);
		}

		return center /= (float)frustumCorners.size();
    }

	glm::mat4 Frustum::CalcFrustumMatrix(const glm::mat4& projection, const glm::mat4& view)
	{
		return glm::inverse(projection * view);
	}

	void Frustum::SetCorners()
	{
		
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					glm::vec4 point
					{
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * z - 1.0f,
						1.0f
					};

					point = m_Matrix * point;

					m_Corners.push_back(point / point.w);
				}
			}
		}

	}

	//void Frustum::Render(std::shared_ptr<Shader> shader)
    //{}
}