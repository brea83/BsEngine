#pragma once
#include "Graphics/Camera.h"


struct CameraComponent
{
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;

	Camera Cam;
	std::string Name{ "Camera Component" };
	bool IsPrimaryCamera{ true };

};

