#pragma once
#include "Core.h"
#include "Application.h"
//#include "EngineContext.h"
#ifdef  PIXIE_PLATFORM_WINDOWS

extern Pixie::Application* Pixie::CreateApplication();

int main(int argc, char** argv)
{

	auto application = Pixie::CreateApplication();
	application->Run();
	delete application;
	return 0;
}

//void PopulateStartingScene(EngineContext* engine)
//{
//	Scene* startingScene = engine->GetScene();

//	GameObject* testObject = new GameObject("Viking House");
//	std::shared_ptr<Transform> gameObjectTransform = testObject->GetTransform();
//	gameObjectTransform->SetScale(glm::vec3(20.0f));
//	startingScene->AddGameObject(testObject);
//	testObject->AddComponent<MeshComponent, const  std::string&>("Assets/Meshes/Viking_House.obj");

//	GameObject* testObject2 = new GameObject("Empty");
//	std::shared_ptr<Transform> gameObjectTransform2 = testObject2->GetTransform();
//	gameObjectTransform2->SetScale(glm::vec3(2.0f));
//	gameObjectTransform2->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
//	startingScene->AddGameObject(testObject2);

//	testObject->AddChild(testObject2);

//	//entt::registry& registry = engine->GetRegistry();
//	//entt::entity testEntity = registry.create();
//	//
//	//Transform transform = registry.emplace<Transform>(testEntity);
//	//MeshComponent mesh = registry.emplace<MeshComponent, PrimitiveMeshType>(testEntity, PrimitiveMeshType::Cube);

//}
#endif //  PIXIE_PLATFORM_WINDOWS
