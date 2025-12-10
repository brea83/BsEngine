#pragma once
#include "BsPrecompileHeader.h"

#include "Events/Event.h"

const unsigned int START_WIDTH = 800;
const unsigned int START_HEIGHT = 600;

struct GLFWwindow;

struct WindowProperties
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProperties(const std::string& title = "Pixie Engine", unsigned int width = 800, unsigned int height = 600)
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
	int WindowWidth() const { return m_Data.Width; }
	/*bool SetWindowWidth(int value)
	{
		bool valueIsPositive = value > 0;
		_windowWidth = valueIsPositive ? value : _windowWidth;
		return valueIsPositive;
	}*/
	int WindowHeight() const { return m_Data.Height; }
	/*bool SetWindowHeight(int value)
	{
		bool valueIsPositive = value > 0;
		_windowHeight = valueIsPositive ? value : _windowHeight;
		return valueIsPositive;
	}*/

	GLFWwindow* GetGlfwWindow() const { return m_Window; }

	void SetEventCallback(const EventCallbackFunction& callback) { m_Data.EventCallback = callback; }

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

	WindowData m_Data;
	GLFWwindow* m_Window{ nullptr };
	//int _windowWidth{ 800 };
	//int _windowHeight{ 600 };


};

