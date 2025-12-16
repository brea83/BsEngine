#pragma once
#include <glm/glm.hpp>
#include "Events/MouseCodes.h"
#include "Events/KeyCodes.h"

namespace Pixie
{
	class Input
	{
	public:
		static bool IsKeyPressed(Inputs::Keyboard keycode) { return s_Instance->IsKeyPressedImplementation(keycode); }
		static bool IsMouseButtonPressed(Inputs::Mouse button) { return s_Instance->IsMouseButtonPressedImplementation(button); }
		static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImplementation(); }

	protected:
		virtual bool IsKeyPressedImplementation(Inputs::Keyboard keycode) = 0;
		virtual bool IsMouseButtonPressedImplementation(Inputs::Mouse button) = 0;
		virtual  glm::vec2 GetMousePositionImplementation() = 0;
	private:
		static Input* s_Instance;
	};

}