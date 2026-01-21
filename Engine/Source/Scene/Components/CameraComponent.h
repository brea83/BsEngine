#pragma once
#include "Core.h"
#include "Graphics/Camera.h"
#include "Resources/FileStream.h"
//#include "Scene/Components/Component.h"


namespace Pixie
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		//static constexpr SerializableComponentID ID{ SerializableComponentID::CameraComponent };
		Camera Cam;
		//std::string Name{ "Camera Component" };
		bool IsActive{ false };
		bool IsDefault{ false };
		bool IsOrthographic() { return Cam.IsOrthographic(); }
		void SetOrthographic(bool value) { Cam.SetOrthographic(value); }
		void LockAspectRatio(bool value = true) { Cam.LockAspectRatio(value); }
		bool IsAspectRatioLocked() const { return Cam.IsAspectRatioLocked(); }


		static void Serialize(StreamWriter* stream, const CameraComponent& component)
		{
			//stream->WriteString(component.Name);
			stream->WriteRaw(component.IsActive);
			stream->WriteRaw(component.IsDefault);
			stream->WriteRaw(component.Cam);
		}
		static bool Deserialize(StreamReader* stream, CameraComponent& component);
		//{
		//	stream->ReadRaw(component.IsActive);
		//	stream->ReadRaw(component.IsDefault);
		//	stream->ReadRaw(component.Cam);
		//	return true;
		//}
	};
}