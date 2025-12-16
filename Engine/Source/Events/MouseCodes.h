#pragma once
//#include <GLFW/glfw3.h>

using MouseCode = uint16_t;

namespace Pixie::Inputs
{
	enum class Mouse: MouseCode
	{
		Button1 = 0, //GLFW_MOUSE_BUTTON_1,
		Button2 = 1, //GLFW_MOUSE_BUTTON_2,
		Button3 = 2, //GLFW_MOUSE_BUTTON_3,
		Button4 = 3, //GLFW_MOUSE_BUTTON_4,
		Button5 = 4, //GLFW_MOUSE_BUTTON_5,
		Button6 = 5, //GLFW_MOUSE_BUTTON_6,
		Button7 = 6, //GLFW_MOUSE_BUTTON_7,
		Button8 = 7, //GLFW_MOUSE_BUTTON_8,
		ButtonLast = 8, //GLFW_MOUSE_BUTTON_LAST,
		ButtonLeft = 0, //GLFW_MOUSE_BUTTON_LEFT,
		ButtonRight = 1, //GLFW_MOUSE_BUTTON_RIGHT,
		ButtonMiddle = 2, //GLFW_MOUSE_BUTTON_MIDDLE,
	};
}
