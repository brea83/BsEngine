#include "EngineContext.h"
#include "ForwardRenderer.h"

EngineContext::EngineContext()
{
	// get default scene and renderer from some kind of ini?
	_activeScene = new Scene();
	_renderer = new ForwardRenderer( this );
	_renderer->Init();
}

EngineContext::EngineContext(Scene* scene, Renderer* renderer/*, DebugConsole* console*/)
	: _activeScene( scene ), _renderer(renderer)
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
