#pragma once
#include "Core.h"
#include "Graphics/Camera.h"


namespace Pixie
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		Camera Cam;
		std::string Name{ "Camera Component" };
		bool IsPrimaryCamera{ false };

	};
}