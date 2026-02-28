#pragma once
#include "Scene/Player/PlayerInput.h"

namespace Pixie
{
	class Player;

	class SplinePlayerInput : public PlayerInputSystem
	{
	public:
		SplinePlayerInput() = default;
		SplinePlayerInput(const SplinePlayerInput&) = default;
		virtual void OnEvent(std::shared_ptr<Scene> scene, GameObject& player, Event& event) override;
	protected:
		GameObject m_CurrentPlayer;
		//PlayerInputComponent* m_InputComponent{ nullptr };
		virtual bool OnMouseMoved(MouseMovedEvent& event)  override;
		virtual bool OnMouseClicked(MouseButtonPressedEvent& event)  override;
		virtual bool OnKeyPressed(KeyPressedEvent& event)  override;
		virtual bool OnKeyUp(KeyReleasedEvent& event)  override;
	};

}