#pragma once
#include "Core.h"
//#include "Application.h"
#include "EngineContext.h"
#ifdef  PIXIE_PLATFORM_WINDOWS

extern Pixie::EngineContext* Pixie::CreateApplication();

int main(int argc, char** argv)
{

	auto engine = Pixie::CreateApplication();
	if (!engine->Init())
	{
		return -1;
	}
	
//#pragma region AddMeshesToStartScene
	//PopulateStartingScene(engine);
//#pragma endregion 1 triangles, 1 cube

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// loop until the user closes window
	while (engine->IsRunning()/*!glfwWindowShouldClose(glfw)*/)
	{
		// events collected durring the prev frame are distributed before the next frame
		engine->DispatchEvents();

		engine->Update();

		//draw previous frame
		engine->Draw();
	}

	return 0;
}
//
//void PopulateStartingScene(EngineContext* engine)
//{
//	Scene* startingScene = engine->GetScene();
//
//	//GameObject* testObject = new GameObject("Viking House");
//	//std::shared_ptr<Transform> gameObjectTransform = testObject->GetTransform();
//	//gameObjectTransform->SetScale(glm::vec3(20.0f));
//	//startingScene->AddGameObject(testObject);
//	//testObject->AddComponent<MeshComponent, const  std::string&>("Assets/Meshes/Viking_House.obj");
//
//	//GameObject* testObject2 = new GameObject("Empty");
//	//std::shared_ptr<Transform> gameObjectTransform2 = testObject2->GetTransform();
//	//gameObjectTransform2->SetScale(glm::vec3(2.0f));
//	//gameObjectTransform2->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
//	//startingScene->AddGameObject(testObject2);
//	//
//	//testObject->AddChild(testObject2);
//
//	entt::registry& registry = startingScene->GetRegistry();
//	GameObject cube1 = startingScene->CreateCube();
//
//	GameObject house = startingScene->CreateEmptyGameObject("Viking House");
//	Transform& transform2 = house.GetComponent<Transform>();
//	transform2.SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
//	transform2.SetScale(glm::vec3(20.0f));
//	house.AddComponent<MeshComponent, const  std::string&>("Assets/Meshes/Viking_House.obj");
//
//
//	cube1.AddChild(house);
//	//Transform transform = registry.get<Transform>(testEntity);
//	//MeshComponent mesh = registry.emplace<MeshComponent, PrimitiveMeshType>(testEntity, PrimitiveMeshType::Cube);
//
//}
#endif //  PIXIE_PLATFORM_WINDOWS
