#pragma once
#include <glm/glm.hpp>
#include "Scene/Components/Component.h"

class Camera 
{
public:
	Camera();

	bool BIsSceneViewCam{ false };

	virtual bool HandleMoveWasd(int keyCode, float deltaTime) = 0;
	virtual bool HandleLookMouse(float xOffset, float yOffset, float deltaTime) = 0;
	bool Zoom(float amount);
	virtual glm::mat4 ViewMatrix() const = 0;
	glm::mat4 ProjectionMatrix() const;

	void SetFov(float fov) { m_Fov = fov;}
	float GetFov() { return m_Fov; }
	void SetAspectRatio(float aspect) { m_AspectRatio = aspect; }
	float GetAspectRatio() { return m_AspectRatio; }
	void SetNearFar(float nearPlane, float farPlane) { m_Near = nearPlane; m_Far = farPlane;}

protected:

	float m_Fov{ 45.0f };
	float m_AspectRatio{ 1280.0f / 720.0f };
	float m_Near{ 0.1f };
	float m_Far{ 100.0f };

	float m_CameraSpeed{ 10.0f };
	float m_MouseLookSesitivity{ 1.0f };

	friend class ImGuiLayer;
	friend class DetailsViewPanel;
};

