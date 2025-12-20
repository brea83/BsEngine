#pragma once
//#include "Scene/GameObject.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "EnTT/entt.hpp"
#include <glm/glm.hpp>

namespace Pixie
{
	class Scene;
	class Event;
	class Camera;
	struct CameraComponent;
	class GameObject;

	// will be used for managing camera switching and blending, as well as forwarding events to appropriate camera controllers
	class CameraManager
	{
	public:
		CameraManager(Scene* scene = nullptr, bool isFlyModeOn = false) : m_Scene(scene), m_EditorFlyMode(isFlyModeOn) {}

		void OnEditorUpdate(float deltaTime);
		void OnPlayUpdate(float deltaTime) {}
		bool OnEvent(Event& event);
		void OnBeginPlayMode();
		//void OnPlayModeUpdate();
		void OnEndPlayMode();

		// start new cameras with an apect ratio that matches the curent rendering viewport
		bool OnCameraAdded(CameraComponent& cameraComponent);
		bool IsCameraRemovable(entt::entity entityToRemove);
		bool OnRemoveCamera(entt::entity entityToRemove);

		void OnViewportSizeEvent(float width, float height);

		Camera* GetActiveCamera();
		Camera* GetActiveCamera(glm::mat4& viewMatrix);
		GameObject GetActiveCameraObject();
		void SetActiveCamera(GameObject& gameObject);
		void SetDefaultCamera(GameObject& gameObject);
		GameObject GetDefaultCamera();

		// Todo Add camera blend stuff. 
		// I think that will mean I'll need update methods for lerping?

	private:
		Scene* m_Scene{ nullptr };
		bool m_EditorFlyMode{ false };

		entt::entity m_ActiveCamera{ entt::null };
		entt::entity m_DefaultCamera{ entt::null };
		entt::entity m_EditorCamera{ entt::null };

		bool OnKeyPressed(KeyPressedEvent& event);

	};

}