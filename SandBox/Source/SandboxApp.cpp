#include <Pixie.h>
#include <Source/EntryPoint.h>
#include "Editor/EditorLayer.h"

class SandboxApp : public Pixie::EngineContext
{
public:
	SandboxApp() : EngineContext()
	{
		SetImGuiLayer(new Pixie::EditorLayer());
	}
	~SandboxApp() {}
};

Pixie::EngineContext* Pixie::CreateApplication()
{

	return new SandboxApp();
}
