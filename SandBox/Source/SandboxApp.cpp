#include <Pixie.h>
#include <Source/EntryPoint.h>

class SandboxApp : public Pixie::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}
};

Pixie::Application* Pixie::CreateApplication()
{

	return new SandboxApp();
}
