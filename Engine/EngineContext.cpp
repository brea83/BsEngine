#include "EngineContext.h"
#include "ForwardRenderer.h"
#include "Camera.h"
#include "Window.h"

EngineContext* EngineContext::_engine = nullptr;

EngineContext::EngineContext()
{
	// get default scene and renderer from some kind of ini?
	_activeScene = new Scene();
	_renderer = new ForwardRenderer( this );
	_renderer->Init();
}

//EngineContext::EngineContext(/*Scene* scene, Renderer* renderer,*/ Window* window/*, DebugConsole* console*/)
///* _activeScene( scene ), _renderer(renderer),*/
//{
//	// get default scene and renderer from some kind of ini?
//	_mainWindow =window;
//	_activeScene = new Scene();
//	_renderer = new ForwardRenderer(this);
//
//}

EngineContext* EngineContext::GetEngine()
{
	if (!_engine)
	{
		_engine = new EngineContext();
		return _engine;
	}
	return _engine;
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
