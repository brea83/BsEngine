#include <Pixie.h>
#include <Source/EntryPoint.h>

class SandboxApp : public Pixie::EngineContext
{
public:
	SandboxApp() : EngineContext(){}
	~SandboxApp() {}
};

Pixie::EngineContext* Pixie::CreateApplication()
{

	return new SandboxApp();
}
