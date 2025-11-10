#include "BsPrecompileHeader.h"
#include "GlfwWrapper.h"
#include "EngineContext.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"

//void FrameBufferSizeCallback1(GLFWwindow* window, int width, int height);

static uint8_t s_GlfwWindowCount = 0;

Window::Window(const WindowProperties& properties)
{
	_data.Title = properties.Title;
	_data.Width = properties.Width;
	_data.Height = properties.Height;
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	--s_GlfwWindowCount;

	if (s_GlfwWindowCount == 0)
	{
		glfwTerminate();
	}
}

int Window::Init()
{
#pragma region WindowInitialization
	_window;
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
	_window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "BsEngine V1", NULL, NULL);
	if (!_window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}
	s_GlfwWindowCount++; //successfully made a window increment our tracker for that

	//make the windows's current context current
	glfwMakeContextCurrent(_window);
	// setting up window  callbacks
	glfwSetWindowUserPointer(_window, &_data);


	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) /*Old Method : FrameBufferSizeCallback1); */
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

	glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		WindowClosedEvent event;
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
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

	glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPosition, double yPosition)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// don't try to run callback if no callback registered
		if (!data.EventCallback) return;

		MouseMovedEvent event((float)xPosition, (float)yPosition);
		data.EventCallback(event);
	});

	glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods)
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

	glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keyCode)
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
	glfwSwapBuffers(_window);
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