#pragma once
#include "Scene/GameObject.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

class Scene;
class Event;
class Camera;
class EditorCamera;

class CameraManager
{
public:
	CameraManager(Scene* scene, bool isFlyModeOn = false) : m_Scene(scene), m_EditorFlyMode(isFlyModeOn) {}

	bool OnEvent(Event& event);
	void OnBeginPlayMode();
	//void OnPlayModeUpdate();
	void OnEndPlayMode();

	void OnEditorUpdate(float deltaTime);

	// start new cameras with an apect ratio that matches the curent rendering viewport
	bool OnCameraAdded(CameraComponent& cameraComponent);
	bool IsCameraRemovable(Entity& entity);
	bool TryRemoveCameraComponent(Entity& entity);

	Camera* GetActiveCamera();
	GameObject& GetActiveCameraObject() { return m_ActiveCamera; }
	void SetActiveCamera(GameObject& gameObject);
	void SetDefaultCamera(GameObject& gameObject);

	// Todo Add camera blend stuff. 
	// I think that will mean I'll need update methods for lerping?

private:
	Scene* m_Scene{ nullptr };
	bool m_EditorFlyMode{ false };

	GameObject m_ActiveCamera{};
	GameObject m_DefaultCamera{};
	GameObject m_EditorCamera{};

	bool OnKeyPressed(KeyPressedEvent& event);

};

