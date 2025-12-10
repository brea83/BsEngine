#include "BsPrecompileHeader.h"
#include "WindowsInput.h"
#include <GLFW/glfw3.h>
#include "EngineContext.h"

Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImplementation(Inputs::Keyboard keycode)
{
	GLFWwindow*	window = EngineContext::GetEngine()->GetGlfwWindow();

	auto state = glfwGetKey(window, static_cast<int32_t>(keycode));

	return state == GLFW_PRESS;
}

bool WindowsInput::IsMouseButtonPressedImplementation(Inputs::Mouse button)
{
	GLFWwindow* window = EngineContext::GetEngine()->GetGlfwWindow();
	auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

	return state == GLFW_PRESS;
}

glm::vec2 WindowsInput::GetMousePositionImplementation()
{
	GLFWwindow* window = EngineContext::GetEngine()->GetGlfwWindow();
	double positionX, positionY;
	glfwGetCursorPos(window, &positionX, &positionY);

	return glm::vec2(positionX, positionY);
}
