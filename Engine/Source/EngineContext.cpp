#include "BsPrecompileHeader.h"
#include "EngineContext.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "GlfwWrapper.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Graphics/Renderers/ForwardRenderer.h"
#include "Editor/ImGuiLayer.h"
#include "Graphics/CameraController.h"
#include "Scene/Components/CameraComponent.h"
#include "Graphics/Primitives/Transform.h"


//#define BIND_EVENT_FUNCTION(x) std::bind(&x, this,  std::placeholders::_1)
//#define BIND_EVENT_FUNCTION(x) [this](auto&&... args) -> decltype(auto){ return this->x(std::forward<decltype(args)>(args)...);}

namespace Pixie
{
	EngineContext* EngineContext::m_Engine = nullptr;
	//EngineContext* EngineContext::NextUID = 0;

	EngineContext::EngineContext(Window* startingWindow, Scene* startingScene, Renderer* startingRenderer)
		: m_MainWindow(startingWindow), m_ActiveScene(startingScene), m_Renderer(startingRenderer)
	{
		if (m_Engine == NULL/* || m_Engine == nullptr*/)
		{
			m_Engine = this;
			return;
		}
		std::cerr << "Error: more than one Engine contexts has been created, only the first is saved as a singleton" << std::endl;
	}

	bool EngineContext::Init()
	{
		if (m_MainWindow == nullptr) m_MainWindow = std::make_shared<Window>();
		// configure glfw and glad state in Window class
		if (!m_MainWindow->Init()) return false;
		m_MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(EngineContext::OnEvent));
		// don't start making render passes if we have no window

		if (m_Renderer == nullptr) m_Renderer = new ForwardRenderer();
		m_Renderer->Init();

		m_ImGuiLayer = new ImGuiLayer();

		if (m_ActiveScene == nullptr) m_ActiveScene = new Scene();
		m_ActiveScene->Initialize();
		m_ActiveScene->GetActiveCamera()->SetAspectRatio((float)m_MainWindow->WindowWidth() / (float)m_MainWindow->WindowHeight());
		m_ImGuiLayer->OnAttach();

		m_PrevMouseX = m_MainWindow->WindowWidth() / 2.0f;
		m_PrevMouseY = m_MainWindow->WindowHeight() / 2.0f;

		return true;
	}

	GLFWwindow* EngineContext::GetGlfwWindow()
	{
		return m_MainWindow->GetGlfwWindow();
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

	glm::vec2 EngineContext::GetViewportSize() const
	{
		return m_ImGuiLayer->GetViewportSize();
	}

	glm::vec2 EngineContext::GetWindowSize() const
	{
		return { glm::vec2(m_MainWindow->WindowWidth(), m_MainWindow->WindowHeight()) };
	}

	void EngineContext::Update()
	{
		//TODO: plan out what gets updated when window is minimized and what doesn't
		if (!m_IsMinimized)
		{
			float currentFrame = (float)glfwGetTime();
			m_DeltaTime = currentFrame - m_LastFrameTime;
			m_LastFrameTime = currentFrame;

			m_ActiveScene->OnUpdate(m_DeltaTime);
		}

		m_ImGuiLayer->OnUpdate(m_DeltaTime);

		//swap glfw buffers and poll events
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
		if (event.GetEventType() == WindowClosedEvent::GetStaticType())
		{
			EnqueEvent<WindowClosedEvent>(static_cast<WindowClosedEvent&>(event));
		}

		if (event.GetEventType() == WindowResizedEvent::GetStaticType())
		{
			EnqueEvent<WindowResizedEvent>(static_cast<WindowResizedEvent&>(event));
		}

		if (event.GetEventType() == KeyPressedEvent::GetStaticType())
		{
			EnqueEvent<KeyPressedEvent>(static_cast<KeyPressedEvent&>(event));
		}

		if (event.GetEventType() == MouseButtonPressedEvent::GetStaticType())
		{
			EnqueEvent<MouseButtonPressedEvent>(static_cast<MouseButtonPressedEvent&>(event));
		}

		if (event.GetEventType() == MouseScrolledEvent::GetStaticType())
		{
			EnqueEvent<MouseScrolledEvent>(static_cast<MouseScrolledEvent&>(event));
		}

		if (event.GetEventType() == MouseMovedEvent::GetStaticType())
		{
			EnqueEvent<MouseMovedEvent>(static_cast<MouseMovedEvent&>(event));
		}
	}

	void EngineContext::DispatchEvents()
	{
		//if(!m_EventQueue.empty()) std::cout << "----------------------------------"  << std::endl;

		while (!m_EventQueue.empty())
		{
			//std::cout << "Event queue count: " << m_EventQueue.size() << std::endl;
			std::shared_ptr<Event> eventptr = m_EventQueue.front();
			DispatchEvent(eventptr);
			m_EventQueue.pop_front();
			//if (m_EventQueue.empty()) std::cout << "Event queue count: " << m_EventQueue.size() << std::endl;
		}
		//if (!m_EventQueue.empty()) 	std::cout << "WARNING EVENT QUEUE NOT EMPTY AFTER DISPATCH, CONTAINS COUNT: " << m_EventQueue.size() << std::endl;
	}

	void EngineContext::DispatchEvent(std::shared_ptr<Event> eventptr)
	{

		Event& event = *eventptr.get();
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

			entt::registry& registry = m_ActiveScene->GetRegistry();
			entt::entity activeCamEntity = m_ActiveScene->GetActiveCameraGameObject();

			CameraController* cameraComponent = registry.try_get<CameraController>(activeCamEntity);
			Transform* cameraTransform = registry.try_get<Transform>(activeCamEntity);

			if (cameraComponent == nullptr || cameraTransform == nullptr) return false;

			return cameraComponent->HandleMouseLook(cameraTransform, xOffset, yOffset, m_DeltaTime);
		}
		return false;
	}

	bool EngineContext::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();


		/*if (Inputs::KeyboardNames.find(key) != Inputs::KeyboardNames.end())
		{
			std::cout << event.ToString() << " named: " << Inputs::KeyboardNames.at(key) << std::endl;
		}
		else
		{
			std::cout << event.ToString() << ", and key not in lookup tables " << std::endl;
		}*/

		// TODO: refactor this into a proper input system
		if (m_CamFlyMode &&
			(keyCode == Inputs::Keyboard::W
				|| keyCode == Inputs::Keyboard::S
				|| keyCode == Inputs::Keyboard::A
				|| keyCode == Inputs::Keyboard::D))
		{
			entt::registry& registry = m_ActiveScene->GetRegistry();
			GameObject activeCamObject = m_ActiveScene->GetActiveCameraGameObject();

			CameraController* cameraComponent = registry.try_get<CameraController>(activeCamObject);
			Transform* cameraTransform = registry.try_get<Transform>(activeCamObject);

			if (cameraComponent == nullptr || cameraTransform == nullptr) return false;

			return cameraComponent->HandleKeyInput(cameraTransform, keyCode, m_DeltaTime);
		}

		if (keyCode == Inputs::Keyboard::Tab)
		{
			//TODO: SERIOUSLY NEED A BETTER WAY THAN THESE HARDCODED THINGS
			ToggleCamFlyMode();
			return false;
		}

		m_ImGuiLayer->OnEvent(event);
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
}