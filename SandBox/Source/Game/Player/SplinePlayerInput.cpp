#include "SplinePlayerInput.h"
#include "Player.h"
namespace Pixie
{
	void SplinePlayerInput::OnEvent(std::shared_ptr<Scene> scene, GameObject& player, Event& event)
	{
		if (!player)
			return;
		m_CurrentPlayer = player;

		EventDispatcher dispatcher{ event };
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(SplinePlayerInput::OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(SplinePlayerInput::OnMouseClicked));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(SplinePlayerInput::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNCTION(SplinePlayerInput::OnKeyUp));
	}


	bool SplinePlayerInput::OnMouseMoved(MouseMovedEvent& event)
	{
		//m_CurrentPlayer.GetComponent<Player>();

		return false;
	}

	bool SplinePlayerInput::OnMouseClicked(MouseButtonPressedEvent& event)
	{
		//m_CurrentPlayer.GetComponent<Player>();

		return false;
	}

	bool SplinePlayerInput::OnKeyPressed(KeyPressedEvent& event)
	{
		Player& player = m_CurrentPlayer.GetComponent<Player>();

		bool bReticleIsValidObject = player.m_Reticle;
		if (!bReticleIsValidObject)
			return false;

		MovementComponent* reticleMover =  player.m_Reticle.TryGetComponent<MovementComponent>();


		Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();

		switch (keyCode)
		{
		case Inputs::Keyboard::W:
			reticleMover->Direction.y = 1;
			return true;
		case Inputs::Keyboard::S:
			reticleMover->Direction.y = -1;
			return true;
		case Inputs::Keyboard::A:
			reticleMover->Direction.x = -1;
			return true;
		case Inputs::Keyboard::D:
			reticleMover->Direction.x = 1;
			return true;
		
		default:
			break;
		}

		if (!event.IsRepeat())
		{
			switch (keyCode)
			{
			case Inputs::Keyboard::Space:
				player.StartBoosting();
				return true;
			case Inputs::Keyboard::LeftShift:
				player.StartBreaking();
				return true;
			}
		}

		return false;
	}

	bool SplinePlayerInput::OnKeyUp(KeyReleasedEvent& event)
	{
		Player& player = m_CurrentPlayer.GetComponent<Player>();

		if (!player.m_Reticle)
			return false;

		MovementComponent* reticleMover = player.m_Reticle.TryGetComponent<MovementComponent>();

		Inputs::Keyboard keyCode = (Inputs::Keyboard)event.GetKeyCode();

		switch (keyCode)
		{
		case Inputs::Keyboard::W:
			reticleMover->Direction.y = reticleMover->Direction.y == 1 ? 0 : reticleMover->Direction.y;
			return true;
		case Inputs::Keyboard::S:
			reticleMover->Direction.y = reticleMover->Direction.y == -1 ? 0 : reticleMover->Direction.y;
			return true;
		case Inputs::Keyboard::A:
			reticleMover->Direction.x = reticleMover->Direction.x == -1 ? 0 : reticleMover->Direction.x;
			return true;
		case Inputs::Keyboard::D:
			reticleMover->Direction.x = reticleMover->Direction.x == 1 ? 0 : reticleMover->Direction.x;
			return true;
		case Inputs::Keyboard::Space:
			player.StopBoosting();
			return true;
		case Inputs::Keyboard::LeftShift:
			player.StopBreaking();
			return true;
		default:
			break;
		}

		return false;
	}
}