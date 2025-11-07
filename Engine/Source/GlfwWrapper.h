#pragma once
#include "BsPrecompileHeader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Events/Event.h"

const unsigned int START_WIDTH = 800;
const unsigned int START_HEIGHT = 600;

struct WindowProperties
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProperties(const std::string& title = "Bs Engine", unsigned int width = 800, unsigned int height = 600)
		: Title(title), Width(width), Height(height)
	{ }
};

class Window
{
public:
	using EventCallbackFunction = std::function<void(Event&)>;
	Window(const WindowProperties& properties = WindowProperties());
	~Window();

	int Init();
	int WindowWidth() const { return _data.Width; }
	/*bool SetWindowWidth(int value)
	{
		bool valueIsPositive = value > 0;
		_windowWidth = valueIsPositive ? value : _windowWidth;
		return valueIsPositive;
	}*/
	int WindowHeight() const { return _data.Height; }
	/*bool SetWindowHeight(int value)
	{
		bool valueIsPositive = value > 0;
		_windowHeight = valueIsPositive ? value : _windowHeight;
		return valueIsPositive;
	}*/

	GLFWwindow* GetGlfwWindow() const { return _window; }

	void SetEventCallback(const EventCallbackFunction& callback) { _data.EventCallback = callback; }

	// behavior methods
	void OnUpdate();


private:
	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		EventCallbackFunction EventCallback;
	};

	WindowData _data;
	GLFWwindow* _window{ nullptr };
	//int _windowWidth{ 800 };
	//int _windowHeight{ 600 };


};

