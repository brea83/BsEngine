#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera();

	bool BIsSceneViewCam{ false };

	void SetPosition(const glm::vec3& position) { _position = position; }
	const glm::vec3& GetPosition() { return _position; }

	void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f ,0.0f));

	glm::mat4 ViewMatrix() const;
	glm::mat4 ProjectionMatrix() const;

	void SetFov(float fov) { _fov = fov;  _bProjectionMatrixDirty = true; }
	float GetFov() { return _fov; }
	void SetAspectRatio(float aspect) { _aspectRatio = aspect; _bProjectionMatrixDirty = true; }
	float GetAspectRatio() { return _aspectRatio; }
	void SetNearFar(float nearPlane, float farPlane) { _near = nearPlane; _far = farPlane;  _bProjectionMatrixDirty = true; }
protected:
	glm::vec3 _position{ 0.0f, 0.0f, 3.0f };
	glm::vec3 _target{ 0.0f };
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _forward;

	float _fov{ 45.0f };
	float _aspectRatio{ 1280.0f / 720.0f };
	float _near{ 0.1f };
	float _far{ 100.0f };
	bool _bProjectionMatrixDirty{ false };
};

