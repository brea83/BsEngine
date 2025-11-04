#include "Window.h"
#include "EngineContext.h"
#include <iostream>

void FrameBufferSizeCallback1(GLFWwindow* window, int width, int height);

Window::Window()
{}

Window::~Window()
{}

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
	//make the windows's current context current
	glfwMakeContextCurrent(_window);
	// setting up window resize callback
	glfwSetFramebufferSizeCallback(_window,  FrameBufferSizeCallback1);
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void FrameBufferSizeCallback1(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	EngineContext* engine = EngineContext::GetEngine();

	if (engine != nullptr)
	{
		engine->OnFrameBufferSize(width, height);
	}
	glViewport(0, 0, width, height);
}