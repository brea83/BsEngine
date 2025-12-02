#include "BsPrecompileHeader.h"
#include "EngineContext.h"

#include "Graphics/Camera.h"
//#include "GlfwWrapper.h"
//#include <GLFW/glfw3.h>

//#define BIND_EVENT_FUNCTION(x) std::bind(&x, this,  std::placeholders::_1)
#define BIND_EVENT_FUNCTION(x) [this](auto&&... args) -> decltype(auto){ return this->x(std::forward<decltype(args)>(args)...);}

EngineContext* EngineContext::m_Engine = nullptr;
//EngineContext* EngineContext::NextUID = 0;

EngineContext::EngineContext(Window* startingWindow, Scene* startingScene, Renderer* startingRenderer)
	: m_MainWindow(startingWindow), m_ActiveScene(startingScene), m_Renderer(startingRenderer)
{ }

bool EngineContext::Init()
{
	// configure glfw and glad state in Window class
	if (!m_MainWindow->Init()) return false;
	m_MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(EngineContext::OnEvent));
	// don't start making render passes if we have no window
	m_Renderer->Init();
	m_ActiveScene->Initialize();
	m_ActiveScene->GetActiveCamera()->SetAspectRatio((float)m_MainWindow->WindowWidth()/ (float)m_MainWindow->WindowHeight());

	m_ImGuiLayer = new ImGuiLayer();
	m_ImGuiLayer->OnAttach();

	m_PrevMouseX = m_MainWindow->WindowWidth() / 2.0f;
	m_PrevMouseY = m_MainWindow->WindowHeight() / 2.0f;

	return true;
}

EngineContext* EngineContext::GetEngine()
{
	if (m_Engine == NULL)
	{
		m_Engine = new EngineContext();
		return m_Engine;
	}
	return m_Engine;
}

void EngineContext::Update()
{
	//TODO: plan out what gets updated when window is minimized and what doesn't
	if (!m_IsMinimized)
	{
		float currentFrame = (float)glfwGetTime();
		m_DeltaTime = currentFrame - m_LastFrameTime;
		m_LastFrameTime = currentFrame;
	}

	m_ImGuiLayer->OnUpdate(m_DeltaTime);

	m_MainWindow->OnUpdate();
}


void EngineContext::Draw()
{
	if (!m_IsMinimized)
	{
		m_Renderer->BeginFrame(*m_ActiveScene);
		m_Renderer->RenderFrame(*m_ActiveScene);
		m_Renderer->EndFrame(*m_ActiveScene);
	}

	m_ImGuiLayer->Begin();
	m_ImGuiLayer->OnImGuiRender();
	m_ImGuiLayer->End();
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
		m_IsMinimized = true;
		return false;
	}
	m_IsMinimized = false;
	m_ActiveScene->GetActiveCamera()->SetAspectRatio((float)width / (float)height);
	return true;
}

bool EngineContext::OnWindowClosed(WindowClosedEvent& event)
{
	m_IsRunning = false;
	return true;
}


bool EngineContext::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
{
	return false;
}

bool EngineContext::OnMouseScrolled(MouseScrolledEvent& event)
{
	float yOffset = event.GetYOffset();
	if (m_CamFlyMode)
	{
		return m_ActiveScene->GetActiveCamera()->Zoom(yOffset);
	}
	return false;
}

bool EngineContext::OnMouseMoved(MouseMovedEvent& event)
{
	if (m_CamFlyMode)
	{
		float xPosition = event.GetX();
		float yPosition = event.GetY();

		if (m_FirstMouse)
		{
			m_PrevMouseX = xPosition;
			m_PrevMouseY = yPosition;
			m_FirstMouse = false;
		}

		float xOffset = xPosition - m_PrevMouseX;
		float yOffset = yPosition - m_PrevMouseY;

		m_PrevMouseX = xPosition;
		m_PrevMouseY = yPosition;

		return m_ActiveScene->GetActiveCamera()->HandleLookMouse(xOffset, yOffset, m_DeltaTime);
	}
	return false;
}

bool EngineContext::OnKeyPressedEvent(KeyPressedEvent& event)
{
	KeyCode keyCode = event.GetKeyCode();
	
	//Inputs::Keyboard key = static_cast<Inputs::Keyboard>(keyCode);

	/*if (Inputs::KeyboardNames.find(key) != Inputs::KeyboardNames.end())
	{
		std::cout << event.ToString() << " named: " << Inputs::KeyboardNames.at(key) << std::endl;
	}
	else
	{
		std::cout << event.ToString() << ", and key not in lookup tables " << std::endl;
	}*/

	// TODO: refactor this into a proper input system
	if ( m_CamFlyMode &&
		(keyCode == GLFW_KEY_W 
		|| keyCode == GLFW_KEY_S
		|| keyCode == GLFW_KEY_A
		|| keyCode == GLFW_KEY_D))
	{
		return m_ActiveScene->GetActiveCamera()->HandleMoveWasd(keyCode, m_DeltaTime);
	}
	
	if (keyCode == GLFW_KEY_TAB)
	{
		//TODO: SERIOUSLY NEED A BETTER WAY THAN THESE HARDCODED THINGS
		ToggleCamFlyMode();
	}
	//m_ActiveScene->GetActiveCamera()->SetAspectRatio((float)width / (float)height);
	return false;
}


void EngineContext::ToggleCamFlyMode()
{
	m_CamFlyMode = !m_CamFlyMode;
	if (m_CamFlyMode)
	{
		m_FirstMouse = true;
		glfwSetInputMode(m_MainWindow->GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(m_MainWindow->GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}