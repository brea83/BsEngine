#include "BsPrecompileHeader.h"
#include "EngineContext.h"

#include "Graphics/Camera.h"
//#include "GlfwWrapper.h"

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this,  std::placeholders::_1)

EngineContext* EngineContext::_engine = nullptr;

EngineContext::EngineContext(Window* startingWindow, Scene* startingScene, Renderer* startingRenderer)
	: _mainWindow(startingWindow), _activeScene(startingScene), _renderer(startingRenderer)
{ }

bool EngineContext::Init()
{
	// configure glfw and glad state in Window class
	if (!_mainWindow->Init()) return false;
	_mainWindow->SetEventCallback(BIND_EVENT_FUNCTION(EngineContext::OnEvent));
	// don't start making render passes if we have no window
	_renderer->Init();
	_activeScene->GetMainCamera()->SetAspectRatio((float)_mainWindow->WindowWidth()/ (float)_mainWindow->WindowHeight());

	_imGuiLayer = new ImGuiLayer();
	_imGuiLayer->OnAttach();
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
	//TODO: plan out what gets updated when window is minimized and what doesn't
	if (!_bMinimized)
	{
		//ProcessInput(_mainWindow->GetGlfwWindow());
		float currentFrame = glfwGetTime();
		_deltaTime = currentFrame - _lastFrameTime;
		_lastFrameTime = currentFrame;
	}

	_imGuiLayer->OnUpdate(_lastFrameTime);

	_mainWindow->OnUpdate();
}


void EngineContext::Draw()
{
	if (!_bMinimized)
	{
		_renderer->BeginFrame(*_activeScene);
		_renderer->RenderFrame(*_activeScene);
		_renderer->EndFrame(*_activeScene);
	}

	_imGuiLayer->Begin();
	_imGuiLayer->OnImGuiRender();
	_imGuiLayer->End();
}
void EngineContext::DrawConsole()
{}

void EngineContext::OnEvent(Event& event)
{
	EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnWindowClosed));
	dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnFrameBufferSize));

	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnKeyPressedEvent));
	
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseScrolled));
	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(EngineContext::OnMouseMoved));
	//std::cout << event.ToString() << std::endl;
}

bool EngineContext::OnFrameBufferSize(WindowResizedEvent& event)
{
	int width = event.GetWidth();
	int height = event.GetHeight();
	if (width < 1 || height < 1)
	{
		_bMinimized = true;
		return false;
	}
	_bMinimized = false;
	_activeScene->GetMainCamera()->SetAspectRatio((float)width / (float)height);
	return true;
}

bool EngineContext::OnWindowClosed(WindowClosedEvent& event)
{
	_bRunning = false;
	return true;
}


bool EngineContext::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
{
	return false;
}

bool EngineContext::OnMouseScrolled(MouseScrolledEvent& event)
{
	return false;
}

bool EngineContext::OnMouseMoved(MouseMovedEvent& event)
{
	return false;
}

bool EngineContext::OnKeyPressedEvent(KeyPressedEvent& event)
{
	KeyCode keyCode = event.GetKeyCode();
	//Inputs::Keyboard key = static_cast<Inputs::Keyboard>(keyCode);

	//std::cout << event.ToString() << " named: " << Inputs::KeyboardNames.at(key) << std::endl;

	// TODO: refactor this into a proper input system
	if (keyCode == GLFW_KEY_W 
		|| keyCode == GLFW_KEY_S
		|| keyCode == GLFW_KEY_A
		|| keyCode == GLFW_KEY_D)
	{
		return _activeScene->GetMainCamera()->HandleMoveInput(keyCode, _deltaTime);
	}
	

	//_activeScene->GetMainCamera()->SetAspectRatio((float)width / (float)height);
	return false;
}