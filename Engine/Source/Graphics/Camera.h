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

	void SetFov(float fov) { _fov = fov;}
	float GetFov() { return _fov; }
	void SetAspectRatio(float aspect) { _aspectRatio = aspect; }
	float GetAspectRatio() { return _aspectRatio; }
	void SetNearFar(float nearPlane, float farPlane) { _near = nearPlane; _far = farPlane;}

protected:

	float _fov{ 45.0f };
	float _aspectRatio{ 1280.0f / 720.0f };
	float _near{ 0.1f };
	float _far{ 100.0f };

	float _cameraSpeed{ 10.0f };
	float _mouseLookSesitivity{ 1.0f };

	friend class ImGuiLayer;
	friend class DetailsViewPanel;
};

