#include <Pixie.h>
#include <Source/EntryPoint.h>
#include "Editor/EditorLayer.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

using namespace Pixie;
class SandboxApp : public Pixie::EngineContext
{
public:
	SandboxApp() : EngineContext()
	{
		SetImGuiLayer(new Pixie::EditorLayer());
	}
	~SandboxApp() {}

protected:
	
};

Pixie::EngineContext* Pixie::CreateApplication()
{

	return new SandboxApp();
}
