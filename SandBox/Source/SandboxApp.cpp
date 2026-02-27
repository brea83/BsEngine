#include <Pixie.h>
#include <Source/EntryPoint.h>
#include "Editor/EditorLayer.h"
#include "Game/RuntimeLayer.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

using namespace Pixie;
class SandboxApp : public Pixie::EngineContext
{
public:
	SandboxApp(bool bEditorMode) : EngineContext(bEditorMode)
	{
		if (bEditorMode)
		{
			SetImGuiLayer(new Pixie::EditorLayer());
		}
		else
		{
			SetImGuiLayer(new Pixie::RuntimeLayer());
		}
	}
	~SandboxApp() {}

protected:
	
};

Pixie::EngineContext* Pixie::CreateApplication(bool bEditorMode)
{

	return new SandboxApp(bEditorMode);
}
