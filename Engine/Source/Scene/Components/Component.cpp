#include "BsPrecompileHeader.h"
#include "Component.h"

//Component::Component(/*GameObject* parent, const std::string& name*/)
////: _parentObject(parent), Name(name)
//{}
namespace Pixie
{
    const char* LightComponent::LightTypeNames[] = {
                "Directional Light",
                "Point Light",
                "Spot Light"
    };
}