#include "BsPrecompileHeader.h"
#include "GlfwWrapper.h"
//#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "EngineContext.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"

//void FrameBufferSizeCallback1(GLFWwindow* window, int width, int height);

static uint8_t s_GlfwWindowCount = 0;

Window::Window(const WindowProperties& properties)
{
	m_Data.Title = properties.Title;
	m_Data.Width = properties.Width;
	m_Data.Height = properties.Height;
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	--s_GlfwWindowCount;

	if (s_GlfwWindowCount == 0)
	{
		glfwTerminate();
	}
}

int Window::Init()
{
#pragma region WindowInitialization
	m_Window;
	/* Init the GLFW library*/

	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << std::endl;
		return -1;
	}
	// window settings set up as done in Learn OpenGl's tutorial
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a windowed mode window and its openGL context
	m_Window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "BsEngine V1", NULL, NULL);
	if (!m_Window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}
	s_GlfwWindowCount++; //successfully made a window increment our tracker for that

	//make the windows's current context current
	glfwMakeContextCurrent(m_Window);
	// setting up window  callbacks
	glfwSetWindowUserPointer(m_Window, &m_Data);


	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) /*Old Method : FrameBufferSizeCallback1); */
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;
		glViewport(0, 0, width, height);

		// if new window size is invalid for renderer, don't update size or send event;
		//if (width < 1 || height < 1) return;

		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;
		WindowResizedEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		WindowClosedEvent event;
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPosition, double yPosition)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		MouseMovedEvent event((float)xPosition, (float)yPosition);
		data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.EventCallback(event);
				break;
			}
		}
	});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		KeyTypedEvent event(keyCode);
		data.EventCallback(event);
	});

#pragma endregion WindowInitialization

	// initialize GLAD to load openGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// if we pass all the initializers we return 1 yay
	return 1;
}

void Window::OnUpdate()
{
	// glfw: swap buffers and poll IO events
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//void FrameBufferSizeCallback1(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	EngineContext* engine = EngineContext::GetEngine();
//
//	if (engine != nullptr)
//	{
//		//engine->OnFrameBufferSize(width, height);
//	}
//	glViewport(0, 0, width, height);
//}