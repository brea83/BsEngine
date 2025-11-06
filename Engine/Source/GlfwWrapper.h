#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int START_WIDTH = 800;
const unsigned int START_HEIGHT = 600;

class Window
{
public:

	Window();
	~Window();

	int Init();
	const int WindowWidth() const { return _windowWidth; }
	bool SetWindowWidth(int value)
	{
		bool valueIsPositive = value > 0;
		_windowWidth = valueIsPositive ? value : _windowWidth;
		return valueIsPositive;
	}
	const int WindowHeight() const { return _windowHeight; }
	bool SetWindowHeight(int value)
	{
		bool valueIsPositive = value > 0;
		_windowHeight = valueIsPositive ? value : _windowHeight;
		return valueIsPositive;
	}

	GLFWwindow* GetGlfwWindow() const { return _window; }

private:
	GLFWwindow* _window{ nullptr };
	int _windowWidth{ 800 };
	int _windowHeight{ 600 };


};

