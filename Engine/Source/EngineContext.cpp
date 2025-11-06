#include "BsPrecompileHeader.h"
#include "EngineContext.h"

#include "Graphics/Camera.h"
//#include "GlfwWrapper.h"

EngineContext* EngineContext::_engine = nullptr;

EngineContext::EngineContext(Window* startingWindow, Scene* startingScene, Renderer* startingRenderer)
	: _mainWindow(startingWindow), _activeScene(startingScene), _renderer(startingRenderer)
{ }

bool EngineContext::Init()
{
	// configure glfw and glad state in Window class
	if (!_mainWindow->Init()) return false;
	// don't start making render passes if we have no window
	_renderer->Init();
	return true;
}

EngineContext* EngineContext::GetEngine()
{
	if (!_engine)
	{
		_engine = new EngineContext();
		return _engine;
	}
	return _engine;
}

void EngineContext::Update()
{

}


void EngineContext::Draw()
{
	_renderer->BeginFrame(*_activeScene);
	_renderer->RenderFrame(*_activeScene);
	_renderer->EndFrame(*_activeScene);
}
void EngineContext::DrawConsole()
{}

void EngineContext::OnFrameBufferSize(int width, int height)
{
	_activeScene->GetMainCamera()->SetAspectRatio((float)width / (float)height);
}

void EngineContext::OnGlfwInput(GLFWwindow* window)
{}
