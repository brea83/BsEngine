#pragma once
#include "Input.h"


class WindowsInput : public Input
{
public:

protected:
	// Inherited via Input
	bool IsKeyPressedImplementation(Inputs::Keyboard keycode) override;
	bool IsMouseButtonPressedImplementation(Inputs::Mouse button) override;
	glm::vec2 GetMousePositionImplementation() override;
};

