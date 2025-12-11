#pragma once
#include "Core.h"
//#include "Application.h"
#include "EngineContext.h"
#ifdef  PIXIE_PLATFORM_WINDOWS

extern Pixie::EngineContext* Pixie::CreateApplication();

int main(int argc, char** argv)
{

	//auto application = Pixie::CreateApplication();
	//application->Run();
	//delete application;

	auto engine = Pixie::CreateApplication();
	if (!engine->Init())
	{
		return -1;
	}
	//GLFWwindow* glfw = engine->GetGlfwWindow();

//#pragma region AddMeshesToStartScene
	//PopulateStartingScene(engine);
	//#pragma endregion 1 triangles, 1 cube

		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// loop until the user closes window
	while (engine->IsRunning()/*!glfwWindowShouldClose(glfw)*/)
	{
		// input moved into engine events
		//ProcessInput(glfw);


		//this is where we do the engine context
		engine->Draw();

		// glfw: swap buffers and poll IO events moved into window class OnUpdate()
		engine->Update();
	}

	// clean up GLFW
	//glfwTerminate();

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
