#include "BsPrecompileHeader.h"
#include "CameraComponent.h"
#include "Component.h"


namespace Pixie
{
	bool CameraComponent::Deserialize(StreamReader* stream, CameraComponent& component)
	{
		//SerializableComponentID readID;
		//stream->ReadRaw<SerializableComponentID>(readID);
		//if (readID != SerializableComponentID::CameraComponent) return false;
		//return true;

		stream->ReadRaw(component.IsActive);
		stream->ReadRaw(component.IsDefault);
		stream->ReadRaw(component.Cam);
		return true;
	}
}