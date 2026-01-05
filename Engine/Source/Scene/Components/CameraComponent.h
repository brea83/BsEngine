#pragma once
#include "Core.h"
#include "Graphics/Camera.h"
#include "Scene/Components/Component.h"


namespace Pixie
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		static constexpr SerializableComponentID ID{ SerializableComponentID::CameraComponent };
		Camera Cam;
		std::string Name{ "Camera Component" };
		bool IsActive{ false };
		bool IsDefault{ false };


		static void Serialize(StreamWriter* stream, const CameraComponent& component)
		{
			stream->WriteRaw<SerializableComponentID>(component.ID);
		}
		static bool Deserialize(StreamReader* stream, CameraComponent& component)
		{
			SerializableComponentID readID;
			stream->ReadRaw<SerializableComponentID>(readID);
			if (readID != component.ID) return false;
			return true;
		}
	};
}